"""Derive an immutable-source filesystem with exact, shared N64 FX records.

N64F v3 keeps each map's index/atlas/colours/explosions; index offsets address
fx/records.bank instead of the map file. No record is edited or approximated.
The bank's CRC32 includes all payload padding. Manifests/stamps stay outside DFS.
"""
from __future__ import annotations
import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import shutil
import struct
import tempfile
import uuid
import zlib

SCHEMA = 'halo64-exact-fx-pool-1'
BANK_PATH = 'fx/records.bank'
BANK_HEADER = struct.Struct('>4s7I')
ATLAS_BYTES = 4224
MAX_RECORD = 2048

def _sha(data):
    return hashlib.sha256(data).hexdigest()

def _align16(size):
    return (size + 15) & ~15

def _span(offset, size, limit, label):
    if offset < 0 or size < 0 or offset > limit or size > limit - offset:
        raise ValueError(f'{label}: out-of-range span')

def parse_map(data):
    """Validate a freshly exported v2 file and retain exact logical bodies."""
    if len(data) < 32:
        raise ValueError('Truncated FX header')
    magic, version, count, cells, index, atlas = struct.unpack_from('>4sIHHII', data)
    explosions, nexpl = struct.unpack_from('>IH', data, 20)
    if magic != b'N64F' or version != 2 or not 0 < cells <= 32:
        raise ValueError('Expected unpooled N64F v2 with a valid atlas')
    if data[27:32] != bytes(5) or data[26] >= 32:
        raise ValueError('Unknown FX reserved/flame fields')
    _span(index, count * 8, len(data), 'FX index')
    _span(atlas, ATLAS_BYTES, len(data), 'FX atlas')
    _span(explosions, nexpl * 4, len(data), 'FX explosions')
    if index < 32 or index + count * 8 > atlas or atlas % 16:
        raise ValueError('Invalid FX index/atlas layout')
    if atlas + ATLAS_BYTES > explosions or explosions + nexpl * 4 != len(data):
        raise ValueError('Invalid FX record/explosion layout')
    records = []
    previous = -1
    intervals = []
    for i in range(count):
        tag, size, offset = struct.unpack_from('>HHI', data, index + i * 8)
        if tag <= previous or not 8 <= size <= MAX_RECORD or offset % 16:
            raise ValueError('Invalid FX tag ordering, record size or alignment')
        previous = tag
        _span(offset, _align16(size), explosions, 'FX padded record')
        if offset < atlas + ATLAS_BYTES:
            raise ValueError('FX record overlaps metadata')
        payload = data[offset:offset + size]
        if any(data[offset + size:offset + _align16(size)]):
            raise ValueError('Unexpected nonzero FX record DMA padding')
        events, emitters = struct.unpack_from('>HH', payload)
        emitter_offset = (4 + 2 * (events + 1) + 3) & ~3
        if emitter_offset > size or emitters * 80 != size - emitter_offset:
            raise ValueError('Invalid FX emitter body size')
        firsts = struct.unpack_from('>' + str(events + 1) + 'H', payload, 4)
        if firsts[0] != 0 or firsts[-1] != emitters or any(a > b for a, b in zip(firsts, firsts[1:])):
            raise ValueError('Invalid FX event ranges')
        records.append(dict(tag=tag, size=size, offset=offset, payload=payload))
        intervals.append((offset, offset + size))
    intervals.sort()
    if any(a[1] > b[0] and a != b for a, b in zip(intervals, intervals[1:])):
        raise ValueError('Partially overlapping FX records')
    return dict(data=data, count=count, cells=cells, index=index, atlas=atlas,
                explosions=explosions, nexpl=nexpl, flame=data[26], records=records)

def pool_maps(maps):
    """Pure deterministic exact-byte pooling; input/output are byte dictionaries."""
    parsed = {name: parse_map(raw) for name, raw in sorted(maps.items())}
    payload = bytearray()
    offsets = {}
    for meta in parsed.values():
        for record in meta['records']:
            body = record['payload']
            if body not in offsets:
                offsets[body] = BANK_HEADER.size + len(payload)
                payload += body
                payload += bytes((-len(payload)) % 16)
    identity = zlib.crc32(payload) & 0xffffffff
    total = BANK_HEADER.size + len(payload)
    if total > 0xffffffff:
        raise ValueError('FX bank exceeds format address space')
    bank = BANK_HEADER.pack(b'N64B', 1, identity, total, BANK_HEADER.size,
                            len(offsets), len(payload), 0) + payload
    made = {}
    equivalence = []
    for name, meta in parsed.items():
        # Preserve every metadata byte, including gaps, unused atlas cells and
        # the explosion list; only documented addresses/version/identity change.
        out = bytearray(meta['data'][:meta['atlas'] + ATLAS_BYTES])
        struct.pack_into('>I', out, 4, 3)
        struct.pack_into('>I', out, 20, len(out))
        struct.pack_into('>I', out, 28, identity)
        for i, record in enumerate(meta['records']):
            struct.pack_into('>I', out, meta['index'] + i * 8 + 4, offsets[record['payload']])
        out += meta['data'][meta['explosions']:]
        made[name] = bytes(out)
        equivalence.append(verify_map(meta['data'], made[name], bank, name))
    return made, bytes(bank), dict(bank_identity=identity, logical_records=sum(p['count'] for p in parsed.values()),
        unique_records=len(offsets), bank_bytes=len(bank), maps=equivalence,
        old_fx_bytes=sum(len(b) for b in maps.values()),
        new_fx_and_bank_bytes=sum(len(b) for b in made.values()) + len(bank),
        exact_fx_savings_bytes=sum(len(b) for b in maps.values()) - sum(len(b) for b in made.values()) - len(bank))

def verify_bank(bank):
    if len(bank) < BANK_HEADER.size:
        raise ValueError('Truncated FX bank')
    magic, version, identity, total, start, unique, size, reserved = BANK_HEADER.unpack_from(bank)
    if (magic != b'N64B' or version != 1 or total != len(bank) or start != 32 or
            size != total - start or reserved or unique > size // 16 or not unique):
        raise ValueError('Invalid FX bank header/length')
    if zlib.crc32(bank[start:]) & 0xffffffff != identity:
        raise ValueError('FX bank CRC32 mismatch')
    return identity

def verify_map(original, packed, bank, name=''):
    """Independent logical-record and metadata comparison after serialization."""
    meta = parse_map(original)
    identity = verify_bank(bank)
    if len(packed) < 32:
        raise ValueError('Truncated packed FX metadata')
    if struct.unpack_from('>I', packed, 4)[0] != 3 or struct.unpack_from('>I', packed, 28)[0] != identity:
        raise ValueError('Packed FX version/bank identity mismatch')
    explosions = struct.unpack_from('>I', packed, 20)[0]
    if explosions != meta['atlas'] + ATLAS_BYTES or len(packed) != explosions + meta['nexpl'] * 4:
        raise ValueError('Packed FX explosion layout mismatch')
    normalized = bytearray(packed[:explosions])
    struct.pack_into('>I', normalized, 4, 2)
    struct.pack_into('>I', normalized, 20, meta['explosions'])
    struct.pack_into('>I', normalized, 28, 0)
    digests = []
    for i, old in enumerate(meta['records']):
        tag, size, offset = struct.unpack_from('>HHI', packed, meta['index'] + i * 8)
        if (tag, size) != (old['tag'], old['size']) or offset < 32 or offset % 16:
            raise ValueError('Packed FX index metadata mismatch')
        _span(offset, _align16(size), len(bank), 'Packed FX padded record')
        body = bank[offset:offset + size]
        if body != old['payload']:
            raise ValueError('Packed FX logical body changed')
        struct.pack_into('>I', normalized, meta['index'] + i * 8 + 4, old['offset'])
        digests.append(dict(tag=tag, bytes=size, old_offset=old['offset'], bank_offset=offset, sha256=_sha(body)))
    if bytes(normalized) != original[:meta['atlas'] + ATLAS_BYTES]:
        raise ValueError('Packed FX map metadata changed')
    if packed[explosions:] != original[meta['explosions']:]:
        raise ValueError('Packed FX explosion metadata changed')
    return dict(map=name, status='EXACT', records=meta['count'], metadata_bytes=meta['atlas'] + ATLAS_BYTES + meta['nexpl'] * 4,
                old_sha256=_sha(original), packed_sha256=_sha(packed), logical_bodies=digests)

def _inventory(root):
    rows = {}
    for path in sorted(root.rglob('*')):
        if path.is_symlink():
            raise ValueError('Symlinks are not accepted in FX packing trees')
        if path.is_dir():
            continue
        if not path.is_file():
            raise ValueError('Non-regular file in FX packing tree')
        stat = path.stat()
        raw = path.read_bytes()
        after = path.stat()
        if (stat.st_size, stat.st_mtime_ns) != (after.st_size, after.st_mtime_ns):
            raise ValueError('Source changed while reading')
        rows[path.relative_to(root).as_posix()] = dict(bytes=len(raw), sha256=_sha(raw))
    return rows

def _separate(left, right):
    return left != right and left not in right.parents and right not in left.parents

def _write_if_changed(path, payload, *, force=False):
    if not force and path.is_file() and path.read_bytes() == payload:
        return False
    path.parent.mkdir(parents=True, exist_ok=True)
    temporary = path.with_name(path.name + '.tmp-' + uuid.uuid4().hex)
    temporary.write_bytes(payload)
    os.replace(temporary, path)
    return True

def _write_stamp(path, manifest, *, force=False):
    payload = (json.dumps(dict(schema=SCHEMA, manifest_sha256=_sha(json.dumps(manifest, sort_keys=True).encode())), sort_keys=True) + '\n').encode()
    _write_if_changed(path, payload, force=force)

def _manifest_bytes(manifest):
    return (json.dumps(manifest, indent=2) + '\n').encode()

def prepare_filesystem(source_fs, packed_fs, map_names, *, stamp=None):
    """Return a JSON-serializable receipt; publish only a verified separate tree.

    A prior tree may be replaced only when its sibling receipt proves ownership
    and every surviving file still matches that receipt (missing files rebuild).
    Unknown/edited files are never overwritten. Source is never mutated.
    """
    source = Path(source_fs).expanduser().resolve()
    target = Path(packed_fs).expanduser().resolve()
    if not source.is_dir() or not _separate(source, target):
        raise ValueError('Source/output must be separate non-overlapping directories')
    names = sorted(map_names)
    if not names or len(set(names)) != len(names) or any(not re.fullmatch(r'[a-z0-9_]+', n) for n in names):
        raise ValueError('Map names must be a unique explicit nonempty list')
    manifest_path = target.with_name(target.name + '.fx-pool.json')
    # Not a Ninja output: survives `ninja -t clean` and proves that a partial
    # remaining tree is ours before any replacement/repair is attempted.
    owner_path = target.with_name(target.name + '.fx-pool.owner.json')
    stamp_path = Path(stamp).expanduser().resolve() if stamp else target.with_name(target.name + '.fx-pool.stamp')
    if stamp_path in (source, target, manifest_path, owner_path) or source in stamp_path.parents or target in stamp_path.parents:
        raise ValueError('Stamp must be outside both filesystem roots and separate from manifest')
    before = _inventory(source)
    fx_names = sorted(Path(p).stem for p in before if p.startswith('fx/') and p.endswith('.fx'))
    if fx_names != names or BANK_PATH in before:
        raise ValueError('Source FX files must exactly match explicit maps and be unpooled')
    prior = None
    if target.exists():
        ownership_path = owner_path if owner_path.is_file() else manifest_path
        if not target.is_dir() or target.is_symlink():
            raise ValueError('Refusing to replace an unowned output tree')
        existing = _inventory(target)
        # Ninja creates declared output parent directories before running us.
        # A recursively verified empty directory contains no data to overwrite.
        if ownership_path.is_file():
            prior = json.loads(ownership_path.read_text())
            prior_root = prior.get('packed_fs')
            if (prior.get('schema') != SCHEMA or not isinstance(prior_root, str)
                    or not Path(prior_root).is_absolute() or Path(prior_root).resolve() != target):
                raise ValueError('Output ownership receipt mismatch')
            expected = prior.get('output_files', {})
            if any(expected.get(p) != row for p, row in existing.items()):
                raise ValueError('Refusing to replace edited/unknown output files')
        elif existing:
            raise ValueError('Refusing to replace an unowned output tree')
        if prior is not None and prior.get('source_files') == before and prior.get('map_names') == names and existing == expected:
            if _inventory(source) != before:
                raise ValueError('Source changed during idempotence check')
            manifest_repaired = _write_if_changed(manifest_path, _manifest_bytes(prior))
            owner_repaired = _write_if_changed(owner_path, _manifest_bytes(prior))
            _write_stamp(stamp_path, prior, force=manifest_repaired or owner_repaired)
            return prior
    maps = {n: (source/'fx'/(n+'.fx')).read_bytes() for n in names}
    made, bank, proof = pool_maps(maps)
    target.parent.mkdir(parents=True, exist_ok=True)
    work = Path(tempfile.mkdtemp(prefix=target.name + '.stage-', dir=target.parent)).resolve()
    backup = target.with_name(target.name + '.previous-' + uuid.uuid4().hex)
    manifest_tmp = manifest_path.with_name(manifest_path.name + '.tmp-' + uuid.uuid4().hex)
    owner_tmp = owner_path.with_name(owner_path.name + '.tmp-' + uuid.uuid4().hex)
    old_sidecars = {p: p.read_bytes() if p.is_file() else None for p in (manifest_path, owner_path)}
    published = False
    try:
        for rel in before:
            dest = work/rel
            dest.parent.mkdir(parents=True, exist_ok=True)
            if rel.startswith('fx/') and rel.endswith('.fx'):
                dest.write_bytes(made[Path(rel).stem])
            else:
                shutil.copyfile(source/rel, dest)
        (work/BANK_PATH).write_bytes(bank)
        after = _inventory(source)
        output = _inventory(work)
        if after != before:
            raise ValueError('Source changed during FX packing')
        non_fx = [p for p in before if not (p.startswith('fx/') and p.endswith('.fx'))]
        if any(output[p] != before[p] for p in non_fx):
            raise ValueError('Non-FX copied file changed')
        for n in names:
            verify_map((source/'fx'/(n+'.fx')).read_bytes(), (work/'fx'/(n+'.fx')).read_bytes(), (work/BANK_PATH).read_bytes(), n)
        manifest = dict(schema=SCHEMA, source_fs=str(source), packed_fs=str(target), map_names=names,
            bank_path=BANK_PATH, bank_sha256=_sha(bank), source_files=before, output_files=output,
            source_unchanged=True, copied_non_fx_files=len(non_fx), copied_non_fx_bytes=sum(before[p]['bytes'] for p in non_fx),
            old_payload_bytes=sum(r['bytes'] for r in before.values()), new_payload_bytes=sum(r['bytes'] for r in output.values()),
            proof=proof)
        manifest_tmp.write_bytes(_manifest_bytes(manifest))
        owner_tmp.write_bytes(_manifest_bytes(manifest))
        # Recheck ownership immediately before the reversible directory swap.
        if target.exists():
            existing = _inventory(target)
            if (prior is None and existing) or (prior is not None and any(prior['output_files'].get(p) != row for p, row in existing.items())):
                raise ValueError('Output changed before publication')
            target.rename(backup)
        try:
            work.rename(target)
            os.replace(manifest_tmp, manifest_path)
            os.replace(owner_tmp, owner_path)
            published = True
        except BaseException:
            if target.exists():
                target.rename(work)
            if backup.exists():
                backup.rename(target)
            for sidecar, old_data in old_sidecars.items():
                if old_data is None:
                    if sidecar.exists():sidecar.unlink()
                else:
                    _write_if_changed(sidecar, old_data)
            raise
        _write_stamp(stamp_path, manifest, force=True)
        return manifest
    finally:
        # All removable directories are exact children of the verified target
        # parent, created by this call; neither source nor target is removed.
        for removable in (work, backup if published else None):
            if removable is not None and removable.exists():
                resolved = removable.resolve()
                if resolved.parent != target.parent or resolved in (source, target) or source in resolved.parents:
                    raise ValueError('Unsafe temporary cleanup path')
                shutil.rmtree(resolved)
        if manifest_tmp.exists():
            manifest_tmp.unlink()
        if owner_tmp.exists():
            owner_tmp.unlink()

def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--filesystem', required=True, type=Path)
    parser.add_argument('--output', required=True, type=Path)
    parser.add_argument('--maps', required=True, nargs='+')
    parser.add_argument('--stamp', type=Path)
    args = parser.parse_args()
    result = prepare_filesystem(args.filesystem, args.output, args.maps, stamp=args.stamp)
    proof = result['proof']
    print(f"FX pool: {len(result['map_names'])} maps, {proof['logical_records']} exact records, "
          f"{proof['unique_records']} unique, {proof['exact_fx_savings_bytes']} bytes saved")
if __name__ == '__main__':
    main()
