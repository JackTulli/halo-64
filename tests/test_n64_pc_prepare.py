"""Source-only normalization tests. No Invader, ffmpeg or ROM tools execute."""

import hashlib
import json
from pathlib import Path
import struct
import subprocess
import tempfile
from types import SimpleNamespace
import unittest
import wave
from unittest.mock import patch
import zlib

from tools.n64 import pc_prepare as prepare


def pc_map(name):
    data = bytearray(0x880)
    data[:4], data[0x7FC:0x800] = b"daeh", b"toof"
    struct.pack_into("<5I", data, 4, 7, len(data), 0, 0x800, 0x80)
    data[0x20:0x20 + len(name)] = name.encode("ascii")
    return bytes(data)


def xbox_map(name, version=5, payload_size=128):
    header, tags = bytearray(0x800), bytearray(payload_size)
    header[:4], header[-4:] = b"daeh", b"toof"
    struct.pack_into("<9I", tags, 0, 0x803A6000 + 36, 0, 0, 1, 0, 0, 0, 0, 0x74616773)
    compressed = zlib.compress(tags)
    padding = (-(0x800 + len(compressed))) % 4096
    struct.pack_into("<5I", header, 4, version, 0x800 + payload_size, padding, 0x800, payload_size)
    header[0x20:0x20 + len(name)] = name.encode("ascii")
    return bytes(header) + compressed + bytes(padding)


def ogg_page(body=b"vorbis", flags=6, serial=123, sequence=0):
    page = bytearray(28)
    page[:4], page[5], page[26], page[27] = b"OggS", flags, 1, len(body)
    struct.pack_into("<II", page, 14, serial, sequence)
    return bytes(page) + body


class FakeDefinitions:
    def fields(self, structure):
        return [(0, SimpleNamespace(name="scaling flags", type="HUDInterfaceScalingFlags", kind="prim"))]


class FakeTools:
    def __init__(self, version=5, thumbnail=True, fail=None):
        self.commands, self.ffmpeg_inputs = [], []
        self.version, self.thumbnail, self.fail = version, thumbnail, fail

    def __call__(self, command, **options):
        self.commands.append(command)
        self_case = Path(command[0]).name
        if self_case == self.fail:
            return subprocess.CompletedProcess(command, 3, b"conversion detail", b"tool failure")
        value = b""
        if self_case == "invader-extract.exe":
            tags = Path(command[command.index("-t") + 1])
            if "-s" in command:
                if self.thumbnail:
                    bitmap = tags / prepare.UI_BITMAP
                    bitmap.parent.mkdir(parents=True, exist_ok=True)
                    bitmap.write_bytes(b"HEK bitmap fixture")
            else:
                (tags / "audio.sound").write_bytes(b"sound tag" + ogg_page())
                (tags / "hud.unit_hud_interface").write_bytes(b"HUD tag")
        elif self_case == "invader-edit.exe":
            if "-C" in command:
                value = b"1"
            elif "-G" in command:
                field = command[-1]
                if field.endswith("use_high_res_scale"):
                    value = b"1"
                elif field.endswith("width_scale"):
                    value = b"0"
                elif field.endswith("height_scale"):
                    value = b"2"
                elif field.endswith("permutations[0].name"):
                    value = b"permutation"
                elif field.endswith(".name"):
                    value = b"pitch"
        elif self_case == "ffmpeg.exe":
            self.ffmpeg_inputs.append(options["input"])
            Path(command[-1]).write_bytes(b"PCM fixture")
        elif self_case == "invader-build.exe":
            target = Path(command[command.index("-o") + 1])
            target.write_bytes(xbox_map(target.stem, self.version))
        return subprocess.CompletedProcess(command, 0, value, b"")


class PrepareTests(unittest.TestCase):
    def setUp(self):
        self.temporary = tempfile.TemporaryDirectory()
        self.addCleanup(self.temporary.cleanup)
        self.root = Path(self.temporary.name).resolve()
        self.maps, self.work, self.tools = (self.root / part for part in ("maps", "work", "trusted tools"))
        self.maps.mkdir()
        self.tools.mkdir()
        for name in (*prepare.EXTRAS, "ui"):
            (self.maps / (name + ".map")).write_bytes(pc_map(name))
        # These are resource-cache files, deliberately not retail map headers.
        for name in ("bitmaps", "sounds"):
            (self.maps / (name + ".map")).write_bytes(b"resource cache fixture")
        for name in prepare.TOOL_NAMES:
            (self.tools / name).write_bytes(("trusted test binary " + name).encode())

    def run_prepare(self, fake=None):
        fake = fake or FakeTools()
        with patch.object(prepare.subprocess, "run", fake), patch("tools.n64.tagdefs.TagDefinitions", FakeDefinitions):
            return prepare.prepare_pc_maps(self.maps, self.work, self.tools, log=lambda message: None)

    def test_working_recipe_argv_and_provenance(self):
        fake = FakeTools()
        output = self.run_prepare(fake)
        self.assertEqual(output.name, "maps")
        self.assertEqual(output.parent.parent, self.work)
        commands = [[Path(command[0]).name, *command[1:]] for command in fake.commands]
        tags, audio = output.parent / "tags", output.parent / "sound-source"
        self.assertEqual(commands[0], ["invader-extract.exe", "-O", "-m", str(self.maps), "-t", str(output.parent / "ui-tags"), "-s", "ui\\shell\\bitmaps\\mp_map_grafix.bitmap", str(self.maps / "ui.map")])
        extracts = [c for c in commands if c[0] == "invader-extract.exe" and "-s" not in c]
        self.assertEqual([Path(c[-1]).stem for c in extracts], list(prepare.EXTRAS))
        for old, new in prepare.RECIPE["conversions"]:
            self.assertIn(["invader-convert.exe", "-O", "-t", str(tags), "-g", old, new, "-b", "*." + old], commands)
            self.assertIn(["invader-refactor.exe", "-t", str(tags), "-M", "no-move", "-g", old, new], commands)
        for kind in ("invalid-indices", "invalid-enums"):
            self.assertIn(["invader-bludgeon.exe", "-t", str(tags), "-T", kind, "-b", "*"], commands)
        self.assertIn(["invader-edit.exe", "-t", str(tags), "vehicles\\scorpion\\headlights scorpion.lens_flare", "-S", "reflections[0].bitmap_index", "0"], commands)
        self.assertIn(["invader-edit.exe", "-t", str(tags), "hud.unit_hud_interface", "-S", "scaling_flags.use_high_res_scale", "0", "-S", "width_scale", "0.5", "-S", "height_scale", "1.0"], commands)
        self.assertIn(["invader-sound.exe", "-t", str(tags), "-d", str(audio), "-F", "16-bit_pcm", "-j", "2", "audio"], commands)
        self.assertEqual(fake.ffmpeg_inputs, [ogg_page()])
        ffmpeg = next(c for c in commands if c[0] == "ffmpeg.exe")
        self.assertEqual(ffmpeg[1:-1], ["-hide_banner", "-loglevel", "error", "-y", "-i", "pipe:0", "-acodec", "pcm_s16le"])
        for name in prepare.EXTRAS:
            self.assertIn(["invader-build.exe", "-g", "xbox-ntsc", "-E", "-H", "-t", str(tags), "-o", str(output / (name + ".map")), f"levels\\test\\{name}\\{name}"], commands)
        manifest = json.loads((output.parent / "manifest.json").read_text())
        self.assertEqual(manifest["status"], "complete")
        self.assertEqual(len(manifest["outputs"]), 6)
        self.assertEqual(len(manifest["tools"]), 8)
        self.assertEqual(len(manifest["inputs"]), 9)
        self.assertEqual(manifest["outputs"][0]["sha256"], hashlib.sha256((output / "dangercanyon.map").read_bytes()).hexdigest())
        self.assertEqual(manifest["tools"][0]["sha256"], hashlib.sha256((self.tools / prepare.TOOL_NAMES[0]).read_bytes()).hexdigest())
        self.assertEqual(manifest["commands"], fake.commands)
        self.assertTrue((output.parent / "ui-tags" / prepare.UI_BITMAP).is_file())
        self.assertEqual(len(list((output.parent / "logs").glob("*.log"))), len(commands))

    def test_vorbis_stage_keeps_non_block_aligned_pcm_lossless(self):
        tags, audio = self.root / "tags", self.root / "sound-source"
        tags.mkdir()
        original = b"sound tag" + ogg_page()
        (tags / "audio.sound").write_bytes(original)
        # 65 frames must stay 65; this fixture would expose ADPCM block truncation.
        pcm = b"".join(struct.pack("<h", value) for value in ([0x1234, -2000, 0, 32767, -32768] * 13))
        calls = []

        def ffmpeg(stream, target):
            self.assertEqual(stream, ogg_page())
            with wave.open(str(target), "wb") as wav:
                wav.setnchannels(1)
                wav.setsampwidth(2)
                wav.setframerate(22050)
                wav.writeframes(pcm)

        def invoke(tool, *arguments):
            calls.append((tool, arguments))
            if tool == "edit":
                if "-C" in arguments:
                    return "1"
                return "permutation" if arguments[-1].endswith("permutations[0].name") else "pitch"
            self.assertEqual(tool, "sound")
            self.assertEqual(arguments[arguments.index("-F") + 1], "16-bit_pcm")
            with wave.open(str(audio / "audio" / "pitch" / "permutation.wav"), "rb") as wav:
                self.assertEqual((wav.getnchannels(), wav.getsampwidth(), wav.getframerate(), wav.getnframes()),
                                 (1, 2, 22050, 65))
                self.assertEqual(wav.readframes(65), pcm)
            return ""

        self.assertEqual(prepare._normalize_vorbis(tags, audio, invoke, ffmpeg), ["audio.sound"])
        self.assertEqual((tags / "audio.sound").read_bytes(), original)
        sound_calls = [arguments for tool, arguments in calls if tool == "sound"]
        self.assertEqual(len(sound_calls), 1)
        self.assertFalse(any("xbox_adpcm" in arguments for _, arguments in calls))

    def test_existing_non_vorbis_sound_is_not_reencoded(self):
        tags, audio = self.root / "tags", self.root / "sound-source"
        tags.mkdir()
        source = tags / "already-native.sound"
        source.write_bytes(b"native sound without a Vorbis stream")
        with patch("builtins.print"), patch.object(prepare, "_ogg_streams") as ogg:
            invoke, ffmpeg = unittest.mock.Mock(), unittest.mock.Mock()
            self.assertEqual(prepare._normalize_vorbis(tags, audio, invoke, ffmpeg), [])
            invoke.assert_not_called()
            ffmpeg.assert_not_called()
            ogg.assert_not_called()
        self.assertEqual(source.read_bytes(), b"native sound without a Vorbis stream")

    def test_every_build_uses_new_work_and_hashes_same_size_source_changes(self):
        first = self.run_prepare()
        source = self.maps / "dangercanyon.map"
        data = bytearray(source.read_bytes())
        data[-1] ^= 1
        source.write_bytes(data)
        second = self.run_prepare()
        self.assertNotEqual(first, second)
        old = json.loads((first.parent / "manifest.json").read_text())["inputs"]
        new = json.loads((second.parent / "manifest.json").read_text())["inputs"]
        self.assertNotEqual(old, new)
        self.assertTrue(first.exists())

    def test_missing_tools_fail_upfront_without_running_or_creating_work(self):
        (self.tools / "ffmpeg.exe").unlink()
        with patch.object(prepare.subprocess, "run") as run:
            with self.assertRaisesRegex(ValueError, "ffmpeg.exe"):
                prepare.prepare_pc_maps(self.maps, self.work, self.tools)
            run.assert_not_called()
        self.assertFalse(self.work.exists())
        with self.assertRaisesRegex(ValueError, "Select a trusted"):
            prepare.validate_tools(None)

    def test_wrong_source_version_and_missing_ui_fail_before_tools_run(self):
        source = self.maps / "dangercanyon.map"
        data = bytearray(source.read_bytes())
        struct.pack_into("<I", data, 4, 609)
        source.write_bytes(data)
        with patch.object(prepare.subprocess, "run") as run:
            with self.assertRaisesRegex(ValueError, "version 609"):
                prepare.prepare_pc_maps(self.maps, self.work, self.tools)
            run.assert_not_called()
        source.write_bytes(pc_map("dangercanyon"))
        (self.maps / "ui.map").unlink()
        with self.assertRaisesRegex(ValueError, "ui.map"):
            self.run_prepare()

    def test_tool_failure_and_wrong_output_version_leave_failed_manifest(self):
        for fake, message in ((FakeTools(fail="invader-convert.exe"), "conversion detail"),
                              (FakeTools(fail="invader-sound.exe"), "conversion detail"),
                              (FakeTools(version=7), "expected Xbox cache version 5"),
                              (FakeTools(thumbnail=False), "required multiplayer thumbnail")):
            with self.subTest(message=message), self.assertRaisesRegex(ValueError, message):
                self.run_prepare(fake)
        manifests = [json.loads(path.read_text()) for path in self.work.glob("*/manifest.json")]
        self.assertEqual(len(manifests), 4)
        self.assertTrue(all(item["status"] == "failed" for item in manifests))

    def test_reject_source_and_tool_work_roots(self):
        for folder in (self.maps, self.maps / "derived", self.tools, self.tools / "derived"):
            with self.subTest(folder=folder), self.assertRaisesRegex(ValueError, "outside"):
                prepare.prepare_pc_maps(self.maps, folder, self.tools)

    def test_cli_writes_explicit_worker_result(self):
        fake = FakeTools()
        with patch.object(prepare.subprocess, "run", fake), patch("tools.n64.tagdefs.TagDefinitions", FakeDefinitions), patch("builtins.print"):
            prepare.main([str(self.maps), "--work", str(self.work), "--tools", str(self.tools)])
        result = json.loads((self.work / "result.json").read_text())
        for value in result.values():
            self.assertTrue(Path(value).is_absolute())
            self.assertTrue(Path(value).is_relative_to(self.work))
            self.assertTrue(Path(value).exists())

    def test_output_header_name_tag_bounds_and_truncation(self):
        target = self.root / "dangercanyon.map"
        good = xbox_map("dangercanyon")
        target.write_bytes(good)
        prepare.validate_xbox_map(target, "dangercanyon")
        mutations = []
        bad = bytearray(good)
        bad[:4] = b"head"
        mutations.append(bad)
        mutations.append(xbox_map("deathisland"))
        bad = bytearray(good)
        struct.pack_into("<I", bad, 16, 0x880)
        mutations.append(bad)
        mutations.append(good[:2100])
        bad = bytearray(good)
        bad[2048:2050] = b"xx"
        mutations.append(bad)
        for bad in mutations:
            target.write_bytes(bad)
            with self.assertRaises(ValueError):
                prepare.validate_xbox_map(target, "dangercanyon")

    def test_validation_streams_high_compression_payload_with_bounded_chunks(self):
        target = self.root / "dangercanyon.map"
        target.write_bytes(xbox_map("dangercanyon", payload_size=3 << 20))
        prepare.validate_xbox_map(target, "dangercanyon")


class SoundSafetyTests(unittest.TestCase):
    def test_reject_tag_derived_path_traversal_windows_names_and_aliases(self):
        for name in ("", ".", "..", "../escape", "..\\escape", "C:escape", "/root", "a/b", "NUL", "com1.wav", "a.", "a ", "\x00"):
            with self.subTest(name=name), self.assertRaises(ValueError):
                prepare._component(name)
        self.assertEqual(prepare._component("normal name_2"), "normal name_2")

    def test_ogg_bounds_complete_and_multipage(self):
        first = ogg_page(flags=2)
        last = ogg_page(flags=4, sequence=1)
        self.assertEqual(prepare._ogg_streams(b"tag prefix" + first + last), [first + last])
        self.assertEqual(prepare._ogg_streams(ogg_page() + b"tag gap" + ogg_page(serial=99)), [ogg_page(), ogg_page(serial=99)])
        for bad in (b"OggS", ogg_page()[:-1], first, first[:27], first + b"gap" + last,
                    first + ogg_page(flags=4, serial=99, sequence=1),
                    first + ogg_page(flags=4, sequence=2), ogg_page(flags=4)):
            with self.subTest(data=bad), self.assertRaises(ValueError):
                prepare._ogg_streams(bad)

    def test_sound_names_cannot_escape_before_ffmpeg_runs(self):
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            tags, audio = root / "tags", root / "audio"
            tags.mkdir()
            (tags / "unsafe.sound").write_bytes(ogg_page())
            def invoke(tool, *args):
                return "1" if "-C" in args else "../escape"
            with self.assertRaisesRegex(ValueError, "Unsafe sound"):
                prepare._normalize_vorbis(tags, audio, invoke, lambda *_: self.fail("ffmpeg must not run"))
            self.assertFalse(audio.exists())


if __name__ == "__main__":
    unittest.main()
