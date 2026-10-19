/* invader-lite: stand-in for libsquish (not linked). DXT decoding goes
   through Invader's bundled bcdec; DXT encoding is not available. */
#ifndef SQUISH_STUB_H
#define SQUISH_STUB_H
#include <cstdint>
#include <cstring>
#include <stdexcept>
#include "bcdec/bcdec.h"
namespace squish {
    typedef unsigned char u8;
    enum { kDxt1 = 1, kDxt3 = 2, kDxt5 = 4, kColourIterativeClusterFit = 256, kSourceBGRA = 1 << 14 };
    inline void CompressImage(const u8 *, int, int, void *, int) {
        throw std::runtime_error("invader-lite: DXT compression (libsquish) is not available");
    }
    /* RGBA out, like squish: four bytes a pixel, rows of width pixels */
    inline void DecompressImage(u8 *rgba, int width, int height, const void *blocks, int flags) {
        const u8 *in = static_cast<const u8 *>(blocks);
        int block_bytes = (flags & kDxt1) ? 8 : 16;
        u8 tile[16 * 4];
        for(int by = 0; by < height; by += 4) {
            for(int bx = 0; bx < width; bx += 4) {
                if(flags & kDxt1) bcdec_bc1(in, tile, 16);
                else if(flags & kDxt3) bcdec_bc2(in, tile, 16);
                else bcdec_bc3(in, tile, 16);
                in += block_bytes;
                for(int y = 0; y < 4 && by + y < height; y++) {
                    for(int x = 0; x < 4 && bx + x < width; x++) {
                        std::memcpy(rgba + ((by + y) * width + bx + x) * 4, tile + (y * 4 + x) * 4, 4);
                    }
                }
            }
        }
    }
}
#endif
