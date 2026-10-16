"""Analytic Xbox ADPCM fixtures; no retail audio or external tools required."""
import struct
import unittest

from tools.n64.sounds import decode_xbox_adpcm
from tools.n64.music import decode_xbox_adpcm as decode_music


def block(predictors, codes, indices=None, padding=0):
    channels = len(predictors)
    indices = indices if indices is not None else [0] * channels
    header = b"".join(struct.pack("<hBB", p, i, 0)
                      for p, i in zip(predictors, indices))
    payload = bytearray()
    for group in range(8):
        for code in codes:
            data = bytearray([code | code << 4] * 4)
            if group == 7:
                data[-1] = code | padding << 4
            payload.extend(data)
    return header + payload


def pcm(data, channels):
    raw = decode_xbox_adpcm(data, channels)
    return struct.unpack("<" + "h" * (len(raw) // 2), raw)


class XboxADPCMTests(unittest.TestCase):
    def test_music_and_effects_agree_for_all_valid_steps_and_codes(self):
        for channels in (1, 2):
            for index in range(89):
                for code in range(16):
                    data = block([1234, -2345][:channels],
                                 [code, code ^ 8][:channels],
                                 [index] * channels, padding=15)
                    self.assertEqual(decode_music(data, channels),
                                     decode_xbox_adpcm(data, channels))

    def test_predictor_starts_each_64_frame_block(self):
        self.assertEqual(pcm(block([1234], [1]), 1), tuple(range(1234, 1298)))
        self.assertEqual(pcm(block([-1234], [9]), 1), tuple(range(-1234, -1298, -1)))

    def test_zero_codes_retain_predictor_at_minimum_step(self):
        self.assertEqual(pcm(block([-3210], [0]), 1), (-3210,) * 64)

    def test_stereo_headers_and_four_byte_groups_keep_channel_order(self):
        actual = pcm(block([100, -100], [1, 9]), 2)
        expected = tuple(v for frame in range(64)
                         for v in (100 + frame, -100 - frame))
        self.assertEqual(actual, expected)

    def test_signed_clipping_preserves_initial_sample(self):
        actual = pcm(block([32766, -32767], [7, 15], [88, 88]), 2)
        self.assertEqual(actual[:2], (32766, -32767))
        self.assertEqual(actual[2:], (32767, -32768) * 63)

    def test_final_high_nibble_is_padding(self):
        quiet = block([456], [1], padding=0)
        loud = block([456], [1], padding=15)
        self.assertEqual(pcm(quiet, 1), pcm(loud, 1))
        changed_low = bytearray(quiet)
        changed_low[-1] = (changed_low[-1] & 0xf0) | 9
        self.assertNotEqual(pcm(quiet, 1)[-1], pcm(changed_low, 1)[-1])

    def test_state_resets_and_output_duration_stays_64_frames_per_block(self):
        first = block([10], [1])
        second = block([-10], [9])
        self.assertEqual(pcm(first + second, 1), pcm(first, 1) + pcm(second, 1))
        for channels in (1, 2):
            data = block([100] * channels, [1] * channels)
            self.assertEqual(len(decode_xbox_adpcm(data * 3, channels)),
                             3 * 64 * channels * 2)

    def test_existing_incomplete_tail_policy_remains(self):
        data = block([100], [1])
        self.assertEqual(decode_xbox_adpcm(data + data[:17], 1),
                         decode_xbox_adpcm(data, 1))


if __name__ == "__main__":
    unittest.main()
