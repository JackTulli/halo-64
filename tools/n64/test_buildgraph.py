"""Focused build graph tests; writes only into temporary directories, never builds a ROM."""
import ast
import json
import os
from pathlib import Path
import re
import shutil
import struct
import subprocess
import sys
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[2]
PC_MAPS = ('dangercanyon', 'deathisland', 'gephyrophobia', 'icefields', 'infinity', 'timberland')


class BuildGraphTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory(prefix='halo graph spaces ')
        self.base = Path(self.temp.name)
        self.env = dict(os.environ, N64_INST=str(self.base / 'sdk with spaces'), TINY3D=str(self.base / 'tiny3d with spaces'))

    def tearDown(self):
        self.temp.cleanup()

    def generate(self, *args, ok=True, name='output with spaces'):
        out = self.base / name
        command = [sys.executable, str(ROOT / 'tools/n64_build.py'), '--build-dir', str(out), *map(str, args)]
        result = subprocess.run(command, cwd=ROOT, env=self.env, capture_output=True, text=True)
        self.assertEqual(result.returncode == 0, ok, result.stdout + result.stderr)
        return out, result

    def pc_inputs(self):
        maps = self.base / 'owned normalized maps'
        maps.mkdir()
        for name in PC_MAPS:
            header = bytearray(2048)
            header[:4] = b'daeh'
            struct.pack_into('<I', header, 4, 5)
            header[32:32+len(name)] = name.encode()
            struct.pack_into('<H', header, 96, 1)
            (maps / (name+'.map')).write_bytes(header)
        bitmap = self.base / 'owned pc pictures.bitmap'
        bitmap.write_bytes(b'graph-path fixture only; not a real bitmap')
        return maps, bitmap

    @staticmethod
    def text(out):
        return (out/'build.ninja').read_text().replace('$\n    ', '').replace('$\n      ', '')

    def test_default_main13_and_campaign_have_no_pc_inputs(self):
        out, _ = self.generate()
        s = self.text(out)
        maps = re.findall(r'^build .*?/filesystem/maps/(\w+)\.map .*?: convert_map ', s, re.M)
        self.assertEqual(len(maps), 14)  # thirteen multiplayer + campaign a30
        self.assertNotIn('timberland.map', s)
        self.assertNotIn('HALO_N64_PC_EXPANSION', s)
        self.assertIn('HALO_N64_CAMPAIGN', s)
        self.assertNotIn('local/', s)
        self.assertNotIn('../scripts/', s)
        self.assertIn('-mfix4300', s)
        self.assertNotIn('-mno-fix4300', s)
        self.assertIn('filesystem/paint/masks.i4', s)
        self.assertIn('filesystem/decals/materials.bank', s)
        self.assertIn('rom_elf_compression = 1', s)
        # The PC expansion's proven profile keeps the game compact and platform fast.
        blocks = s.split("\nbuild ")
        game = next(b for b in blocks if '/obj/source/game/game.o: cc ' in b)
        renderer = next(b for b in blocks if '/obj/port/n64/src/n64_render.o: cc ' in b)
        game_flags = game.split('cflags = ', 1)[1].split()
        renderer_flags = renderer.split('cflags = ', 1)[1].split()
        self.assertIn('-Os', game_flags)
        self.assertNotIn('-O2', game_flags)
        self.assertIn('-O2', renderer_flags)
        self.assertNotIn('-Os', renderer_flags)

    def test_asset_dependencies_exclude_only_installer_and_test_tools(self):
        out, _ = self.generate()
        s = self.text(out)
        edge = next(line for line in s.splitlines() if ': convert_map ' in line)
        for name in ('rombuilder.py', 'rombuilder_gui.py', 'rombuilder_main.py',
                     'package_rombuilder.py', 'pc_input.py', 'pc_prepare.py', 'xiso.py', 'fx_pool.py',
                     'test_buildgraph.py', 'test_portable_assets.py'):
            self.assertNotIn('tools/n64/' + name, edge)
        for name in ('convert.py', 'bitmaps.py', 'tagdefs.py', 'toolpaths.py',
                     'paint_masks.py', 'decal_bank.py', 'pc_thumbnails.py'):
            self.assertIn('tools/n64/' + name, edge)
        self.assertIn('p8_palette', edge)

        # Guard the architectural boundary if a future converter adds a new import.
        modules = {p.stem: p for p in (ROOT / 'tools/n64').glob('*.py')}
        pending = ['convert', 'level', 'ui', 'music', 'compress_map', 'paint_masks',
                   'decal_bank', 'pc_thumbnails', 'soundbank', 'prune']
        reached = set()
        while pending:
            name = pending.pop()
            if name in reached or name not in modules:
                continue
            reached.add(name)
            self.assertFalse(name.startswith(('rombuilder', 'test_')))
            self.assertNotIn(name, {'package_rombuilder', 'pc_input', 'pc_prepare', 'xiso'})
            for node in ast.walk(ast.parse(modules[name].read_text(encoding='utf-8'))):
                if isinstance(node, ast.ImportFrom):
                    if node.level == 1:
                        pending.extend([node.module.split('.')[0]] if node.module else
                                       [a.name.split('.')[0] for a in node.names])
                    elif node.module and node.module.startswith('tools.n64.'):
                        pending.append(node.module.split('.')[2])
                elif isinstance(node, ast.Import):
                    pending.extend(a.name.split('.')[2] for a in node.names
                                   if a.name.startswith('tools.n64.'))
                elif isinstance(node, ast.Call):
                    called = node.func.id if isinstance(node.func, ast.Name) else (
                        node.func.attr if isinstance(node.func, ast.Attribute) else '')
                    self.assertNotIn(called, {'__import__', 'import_module', 'run_module', 'run_path', 'exec', 'eval'})

    def test_pc_inputs_required_before_creating_any_output(self):
        out, _ = self.generate('--pc-expansion', ok=False)
        self.assertFalse(out.exists())
        out, _ = self.generate('--pc-maps', self.base, ok=False)
        self.assertFalse(out.exists())

    def test_missing_thumbnail_is_explicit_failure(self):
        maps, _ = self.pc_inputs()
        out, result = self.generate('--pc-expansion', '--pc-maps', maps, ok=False)
        self.assertIn('--pc-ui-bitmap', result.stderr)
        self.assertFalse(out.exists())

    def test_pc_variant_isolated_dependencies_and_package_settings(self):
        maps, bitmap = self.pc_inputs()
        out, _ = self.generate('--pc-expansion', '--pc-maps', maps, '--pc-ui-bitmap', bitmap)
        s = self.text(out)
        self.assertIn('pc-expansion/halo.elf', s)
        self.assertIn('halo-pc-expansion.z64', s)
        self.assertIn('HALO_N64_PC_EXPANSION', s)
        pc_maps = re.findall(r'^build .*?/pc-expansion/filesystem/maps/(\w+)\.map .*?: convert_map ', s, re.M)
        self.assertEqual(len(pc_maps), 19)
        self.assertIn('title = HALO 64 PC EXPANSION', s)
        self.assertIn('rom_elf_compression = 0', s)
        self.assertEqual(s.count(': pc_thumbnails '), 1)
        for index in range(13, 19):
            self.assertIn(f'/ui/map{index}.sprite', s)
        # The UI exports a placeholder map13 too: its regeneration must force the PC overwrite afterward.
        thumbnail_edge = next(line for line in s.splitlines() if ': pc_thumbnails ' in line)
        self.assertIn('/ui/scene.bin', thumbnail_edge)
        self.assertNotIn(' || ', thumbnail_edge)
        links = [line for line in s.splitlines() if ': link ' in line]
        self.assertEqual(len(links), 3)
        for line in links:
            for name in ('libdragon.a', 'libm.a', 'libdragonsys.a', 'libc.a', 'n64.ld', 'crt0.o', 'libt3d.a'):
                self.assertIn(name, line)
        map_edges = [line for line in s.splitlines() if ': convert_map ' in line]
        self.assertTrue(all('p8_palette' in line for line in map_edges))
        self.assertTrue(all('compress_map.py' in line for line in map_edges))
        self.assertTrue(all('mkasset' in line for line in map_edges))
        # Native Ninja can parse the graph even though execution requires the upstream POSIX/MSYS shell.
        ninja = shutil.which('ninja')
        if ninja:
            result = subprocess.run([ninja, '-f', str(out/'build.ninja'), '-t', 'commands'], cwd=ROOT,
                                    capture_output=True, text=True)
            self.assertEqual(result.returncode, 0, result.stderr)
            self.assertIn("'", result.stdout)
            self.assertIn('sdk with spaces', result.stdout)
            self.assertNotIn('\\', result.stdout)

    def test_pc_shared_fx_stage_preserves_raw_assets_and_converter_cache(self):
        maps, bitmap = self.pc_inputs()
        out, _ = self.generate('--pc-expansion', '--pc-maps', maps, '--pc-ui-bitmap', bitmap)
        text = self.text(out)
        edges = [line for line in text.splitlines() if line.startswith('build ')]
        pool = [line for line in edges if ': fx_pool ' in line]
        self.assertEqual(len(pool), 1)
        outputs, inputs = pool[0].split(': fx_pool ', 1)
        self.assertIn('tools/n64/fx_pool.py', inputs)
        self.assertIn('/fx-pool-validate', inputs)
        self.assertEqual(inputs.count('/filesystem/fx/'), 19)
        self.assertEqual(outputs.count('/filesystem-packed/fx/'), 20)
        self.assertIn('filesystem-packed.fx-pool.stamp', outputs)
        self.assertIn('filesystem-packed.fx-pool.json', outputs)
        self.assertNotIn('.owner.json', outputs)
        self.assertNotIn('/filesystem-packed/', inputs)
        self.assertTrue(any('/fx-pool-validate: phony' in line for line in edges))
        self.assertIn('restat = 1', text.split('rule fx_pool', 1)[1].split('rule paint_masks', 1)[0])
        converters = [line for line in edges if ': convert_map ' in line]
        self.assertTrue(all('fx_pool.py' not in line for line in converters))
        dfs = [line for line in edges if ': mkdfs' in line]
        self.assertEqual(len(dfs), 3)
        pc = next(line for line in dfs if '/pc-expansion/halo.dfs:' in line)
        self.assertIn(': mkdfs_packed ', pc)
        self.assertIn('/filesystem-packed/', pc)
        readonly_rule = text.split('rule mkdfs_packed', 1)[1].split('rule fx_pool', 1)[0]
        self.assertNotIn('tools.n64.prune', readonly_rule)
        self.assertNotIn('/filesystem/', pc)
        self.assertTrue(all('/filesystem-packed/' not in line for line in dfs if line != pc))

    def test_pc_header_mismatch_rejected(self):
        maps, bitmap = self.pc_inputs()
        (maps/'timberland.map').write_bytes(b'not a normalized map')
        out, _ = self.generate('--pc-expansion', '--pc-maps', maps, '--pc-ui-bitmap', bitmap, ok=False)
        self.assertFalse(out.exists())


if __name__ == '__main__':
    unittest.main()
