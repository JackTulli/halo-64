#!/usr/bin/env python3
"""Generate the header that gives clang MSVC's C linkage semantics.

Two MSVC behaviours that the game (and the XDK headers) depend on have no
compiler switch in clang for ELF targets. This script scans the sources and
writes a header, force-included into every Linux translation unit, that
reproduces both:

1. File-scope struct tags. MSVC gives a struct/union tag that first appears
   inside a function prototype file scope, so a later definition or prototype
   names the same type. Standard C scopes it to the prototype alone, turning
   matching-clean declarations into "conflicting types" errors. Every tag is
   therefore forward-declared at file scope up front. An incomplete forward
   declaration is harmless when the tag is later defined or never used.

2. COMDAT inline functions. MSVC emits a C `__inline` function that has
   external linkage as a pick-any COMDAT. halo_linux_prefix.h makes `__inline`
   static, which is right for most header inlines, but a function that also
   has an ordinary prototype keeps external linkage (an MSVC extension clang
   honours) and would be defined in every unit. `#pragma weak` on each inline
   function name turns exactly those definitions into weak symbols - the ELF
   analogue of a COMDAT - and is ignored for the static ones.
   A weak *reference* to a name that nothing defines would silently resolve
   to NULL, so tools/linux_build.py rejects a link with weak undefined
   symbols.
"""

import argparse
import re
import sys
from pathlib import Path
from typing import Any, Dict, Iterable, List, Set

TAG = re.compile(r"\b(struct|union)\s+([A-Za-z_]\w*)")
COMMENT = re.compile(r"/\*.*?\*/|//[^\n]*", re.S)
INLINE_FUNCTION = re.compile(
    r"\b(?:__inline|_inline|__forceinline|D3DXINLINE|FORCEINLINE)\b"
    r"[^;{}()]*?\b([A-Za-z_]\w*)\s*\([^;{}]*\)\s*\{"
)
SOURCE_SUFFIXES = {".c", ".h", ".inl"}


def source_files(roots: Iterable[Path]) -> List[Path]:
    files: List[Path] = []
    for root in roots:
        files.extend(
            path
            for path in sorted(root.rglob("*"))
            if path.suffix.lower() in SOURCE_SUFFIXES and path.is_file()
        )
    return files


# Conditions that are never true for this build: C++ and 64-bit Windows.
UNUSED_CONDITION = r"(?:__cplusplus|_WIN64)\b"
CPLUSPLUS_IF = re.compile(
    r"#\s*if(?:def\s+" + UNUSED_CONDITION + r"|\s+defined\s*\(?\s*" + UNUSED_CONDITION + r")"
)
CPLUSPLUS_IFNDEF = re.compile(
    r"#\s*if(?:ndef\s+" + UNUSED_CONDITION + r"|\s+!\s*defined\s*\(?\s*" + UNUSED_CONDITION + r")"
)


def strip_cplusplus(text: str) -> str:
    """Drop lines only a C++ or Win64 compiler sees (#ifdef __cplusplus, _WIN64)."""
    kept: List[str] = []
    # One [kind, in_else] entry per open conditional. kind is "cpp" for
    # #ifdef __cplusplus, "c" for #ifndef __cplusplus, else "other".
    stack: List[List[Any]] = []

    def cplusplus_only(entry: List[Any]) -> bool:
        kind, in_else = entry
        return (kind == "cpp" and not in_else) or (kind == "c" and in_else)

    for line in text.splitlines():
        directive = re.sub(r"^#\s*", "#", line.strip())
        if directive.startswith("#if"):
            if CPLUSPLUS_IF.match(directive):
                stack.append(["cpp", False])
            elif CPLUSPLUS_IFNDEF.match(directive):
                stack.append(["c", False])
            else:
                stack.append(["other", False])
            continue
        if directive.startswith("#else") and stack:
            stack[-1][1] = True
            continue
        if directive.startswith("#elif") and stack:
            # an #elif leaves the __cplusplus test behind
            stack[-1] = ["other", False]
            continue
        if directive.startswith("#endif") and stack:
            stack.pop()
            continue
        if not any(cplusplus_only(entry) for entry in stack):
            kept.append(line)
    return "\n".join(kept)


def read(path: Path) -> str:
    return strip_cplusplus(COMMENT.sub(" ", path.read_text(encoding="latin-1")))


def scan_tags(files: Iterable[Path]) -> Dict[str, Set[str]]:
    tags: Dict[str, Set[str]] = {}
    for path in files:
        for kind, name in TAG.findall(read(path)):
            tags.setdefault(name, set()).add(kind)
    return tags


def scan_inline_functions(files: Iterable[Path], all_inlines: bool, exclude_static: bool = False) -> Set[str]:
    """Inline function names that need `#pragma weak`.

    With all_inlines, every one: port/linux/game/msvc_comdat.c gives all of
    them external definitions, which must never clash with an ordinary
    definition elsewhere. Otherwise only those also declared or called
    without a body - the ones that can keep external linkage in an ordinary
    unit - since clang warns about the pragma for names that end up static.
    A call statement also matches that pattern, erring towards the pragma.
    """
    texts = [read(path) for path in files]
    inline_names: Set[str] = set()
    static_names: Set[str] = set()
    for text in texts:
        for match in INLINE_FUNCTION.finditer(text):
            inline_names.add(match.group(1))
            # gcc (the N64 build) rejects `#pragma weak` on a function the
            # source itself declares `static __inline`
            if re.search(r"\bstatic\s*$", text[max(0, match.start() - 40):match.start()]):
                static_names.add(match.group(1))
    if exclude_static:
        inline_names -= static_names
    if all_inlines:
        return inline_names
    everything = "\n".join(texts)
    return {
        name
        for name in inline_names
        if re.search(r"\b" + re.escape(name) + r"\s*\([^;{}]*\)\s*;", everything)
    }


def render(tags: Dict[str, Set[str]], inline_functions: Set[str]) -> str:
    lines = [
        "/* generated by tools/linux_msvc_semantics.py - do not edit */",
        "#ifndef __HALO_LINUX_MSVC_SEMANTICS_H",
        "#define __HALO_LINUX_MSVC_SEMANTICS_H",
        "",
        "/* ---------- file-scope struct and union tags */",
        "",
    ]
    for name in sorted(tags):
        kinds = tags[name]
        if len(kinds) != 1:
            # Used as both struct and union somewhere (third-party code with
            # its own meaning). Leave it to ordinary C scoping.
            continue
        lines.append(f"{next(iter(kinds))} {name};")
    lines += ["", "/* ---------- COMDAT inline functions */", ""]
    for name in sorted(inline_functions):
        lines.append(f"#pragma weak {name}")
    lines += ["", "#endif", ""]
    return "\n".join(lines)


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument("--output", type=Path, required=True)
    parser.add_argument(
        "--tags", type=Path, action="append", default=[],
        help="directory whose struct/union tags are forward-declared",
    )
    parser.add_argument(
        "--inlines", type=Path, action="append", default=[],
        help="directory whose inline functions get COMDAT-like linkage",
    )
    parser.add_argument(
        "--all-inlines", action="store_true",
        help="mark every inline function weak, not just prototyped ones",
    )
    parser.add_argument(
        "--exclude-static", action="store_true",
        help="leave out inline functions the source declares static (gcc)",
    )
    args = parser.parse_args()
    for root in args.tags + args.inlines:
        if not root.is_dir():
            sys.exit(f"{root} is not a directory")
    tag_files = source_files(args.tags)
    inline_files = source_files(args.inlines)
    text = render(scan_tags(tag_files), scan_inline_functions(inline_files, args.all_inlines, args.exclude_static))
    args.output.parent.mkdir(parents=True, exist_ok=True)
    if not args.output.exists() or args.output.read_text(encoding="utf-8") != text:
        args.output.write_text(text, encoding="utf-8")


if __name__ == "__main__":
    main()
