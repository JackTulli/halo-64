"""Small synthetic malformed-cache fixtures; no commercial data required."""
import io
from pathlib import Path
import struct
import tempfile
from types import SimpleNamespace
import unittest
from unittest.mock import patch
import zlib
from tools.n64 import xboxmap


def cache_bytes(name=b'timberland', body_size=4096, padding=0):
    header=bytearray(2048)
    header[:4]=b'daeh';header[-4:]=b'toof'
    struct.pack_into('<5I', header, 4, 5, 2048+body_size, padding, 2048, 72)
    header[32:32+len(name)]=name
    struct.pack_into('<H',header,96,1)
    body=bytearray(body_size)
    struct.pack_into('<9I',body,0,xboxmap.TAG_BASE+36,0,0,1,0,0,0,0,int.from_bytes(b'sgat','little'))
    struct.pack_into('<8I',body,36,0,0,0,0,xboxmap.TAG_BASE+68,xboxmap.TAG_BASE,0,0)
    body[68:72]=b'foo\0'
    return bytes(header)+zlib.compress(body)+bytes(padding),bytes(header)+body


class XboxMapSecurityTests(unittest.TestCase):
    def setUp(self):
        self.temporary=tempfile.TemporaryDirectory()
        self.addCleanup(self.temporary.cleanup)
        self.path=Path(self.temporary.name)/'fixture.map'

    def read(self, raw):
        self.path.write_bytes(raw)
        return xboxmap.read_xbox_map(self.path)

    def test_valid_unpadded_and_maximum_zero_padding_exact_bytes(self):
        for padding in (0,1,4095):
            raw, expected=cache_bytes(padding=padding)
            self.assertEqual(self.read(raw),expected)
            self.assertEqual(xboxmap.XboxMap(self.path).data,expected)

    def test_high_compression_decodes_in_bounded_chunks(self):
        raw,expected=cache_bytes(body_size=3<<20)
        real=zlib.decompressobj
        requests=[]
        class Decoder:
            def __init__(self): self.inner=real()
            def __getattr__(self,key): return getattr(self.inner,key)
            def decompress(self,block,max_length=0):
                requests.append(max_length)
                return self.inner.decompress(block,max_length)
        with patch.object(xboxmap.zlib,'decompressobj',Decoder):
            self.assertEqual(self.read(raw),expected)
        self.assertTrue(requests)
        self.assertTrue(all(0 < n <= 1<<20 for n in requests))

    def test_declared_inflation_overrun_rejected_after_only_one_extra_byte(self):
        raw,_=cache_bytes(body_size=1<<20)
        bad=bytearray(raw);struct.pack_into('<I',bad,8,4096)
        real=zlib.decompressobj
        emitted=[]
        class Decoder:
            def __init__(self): self.inner=real()
            def __getattr__(self,key): return getattr(self.inner,key)
            def decompress(self,block,max_length=0):
                output=self.inner.decompress(block,max_length);emitted.append(len(output));return output
        with patch.object(xboxmap.zlib,'decompressobj',Decoder):
            with self.assertRaisesRegex(ValueError,'exceeds its declared'):
                self.read(bad)
        self.assertEqual(sum(emitted),2049)

    def test_bad_header_signature_version_footer_name_and_bounds(self):
        raw,_=cache_bytes()
        mutations=[]
        for offset,value in ((4,7),(8,2048),(8,xboxmap.MAX_MAP_BYTES+1),(12,4096),(16,2047),(20,35),(20,1<<30)):
            b=bytearray(raw);struct.pack_into('<I',b,offset,value);mutations.append(b)
        for start,value in ((0,b'head'),(2044,b'foot'),(32,b'x'*32),(32,b'../escape\0'),(32,b'NUL\0')):
            b=bytearray(raw);b[start:start+len(value)]=value;mutations.append(b)
        mutations.extend((raw[:100],raw[:2048]))
        for bad in mutations:
            with self.subTest(bytes=len(bad)),self.assertRaises(ValueError):self.read(bad)

    def test_invalid_stream_length_eof_padding_and_second_stream(self):
        raw,_=cache_bytes()
        mutations=[raw[:-1],raw+b'\0',raw+zlib.compress(b'second')]
        bad=bytearray(raw);bad[2048:2050]=b'xx';mutations.append(bad)
        bad=bytearray(raw);struct.pack_into('<I',bad,8,7000);mutations.append(bad)
        padded,_=cache_bytes(padding=2)
        mutations.extend((padded[:-1],padded[:-1]+b'x'))
        for bad in mutations:
            with self.subTest(bytes=len(bad)),self.assertRaises(ValueError):self.read(bad)

    def test_compressed_file_cap_checked_before_header_read(self):
        stream=io.BytesIO(b'not read')
        stream.fileno=lambda: 7
        with patch.object(xboxmap.os,'fstat',return_value=SimpleNamespace(st_size=xboxmap.MAX_MAP_FILE_BYTES+1)),patch.object(Path,'open',return_value=stream),patch.object(stream,'read',wraps=stream.read) as read:
            with self.assertRaisesRegex(ValueError,'compressed size'):
                xboxmap.read_xbox_map(self.path)
            read.assert_not_called()

    def test_tag_table_bounds_checked_before_tag_iteration(self):
        raw,expected=cache_bytes()
        for offset,value in ((2048,xboxmap.TAG_BASE+35),(2048,xboxmap.TAG_BASE+72),(2052,1),(2060,0),(2060,65536),(2080,0)):
            bad=bytearray(expected);struct.pack_into('<I',bad,offset,value)
            packed=bytes(bad[:2048])+zlib.compress(bad[2048:])
            with self.subTest(offset=offset,value=value),self.assertRaisesRegex(ValueError,'tag table'):
                self.read(packed)

    def test_changed_file_metadata_rejected(self):
        raw,_=cache_bytes();self.path.write_bytes(raw)
        real=xboxmap.os.fstat
        calls=0
        def changed(fd):
            nonlocal calls
            calls+=1
            value=real(fd)
            return SimpleNamespace(st_size=value.st_size,st_mtime_ns=value.st_mtime_ns+(calls>1))
        with patch.object(xboxmap.os,'fstat',changed),self.assertRaisesRegex(ValueError,'changed'):
            xboxmap.read_xbox_map(self.path)

if __name__=='__main__':unittest.main()
