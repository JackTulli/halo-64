/* Private-data-free host test of the maintained N64F/N64B byte-level guards.
   cc -std=c99 -Iport/n64/include -Iport/n64/src tests/test_n64_fx_format.c -o test-fx-format */
#include <assert.h>
#include <string.h>
#include "n64_fx_format.h"
static void put16(uint8_t *p, unsigned v) { p[0]=v>>8; p[1]=v; }
static void put32(uint8_t *p, uint32_t v) { p[0]=v>>24;p[1]=v>>16;p[2]=v>>8;p[3]=v; }
int main(void)
{
    uint8_t h[32]={0}, b[32]={0}, record[96]={0};
    struct n64_fx_layout layout;
    uint32_t low, high, crc;
    assert((n64_fx_crc32_update(0xffffffffu,(const uint8_t *)"123456789",9)^0xffffffffu)==0xcbf43926u);
    memcpy(h,"N64F",4);put32(h+4,2);put16(h+8,2);put16(h+10,1);
    put32(h+12,32);put32(h+16,48);put32(h+20,4464);put16(h+24,1);
    assert(n64_fx_map_header(h,4468,&layout));
    put32(h+4,1);put32(h+20,0);put16(h+24,0);
    assert(n64_fx_map_header(h,4464,&layout));
    put16(record,1);put16(record+2,1);put16(record+6,1);
    assert(n64_fx_record_memory(record,88));
    put16(record+6,2);assert(!n64_fx_record_memory(record,88));put16(record+6,1);
    crc=n64_fx_crc32_update(0xffffffffu,record,sizeof(record))^0xffffffffu;
    memcpy(b,"N64B",4);put32(b+4,1);put32(b+8,crc);put32(b+12,128);
    put32(b+16,32);put32(b+20,1);put32(b+24,96);
    assert(n64_fx_bank_header(b,128,crc,&low,&high));assert(low==32&&high==128);
    assert(!n64_fx_bank_header(b,127,crc,&low,&high));
    assert(!n64_fx_bank_header(b,128,crc^1,&low,&high));
    put32(h+4,3);put32(h+20,4272);put16(h+24,1);put32(h+28,crc);
    assert(n64_fx_map_header(h,4276,&layout));
    assert(!n64_fx_map_header(h,4275,&layout));
    h[27]=1;assert(!n64_fx_map_header(h,4276,&layout));h[27]=0;
    assert(n64_fx_record_span(32,128,32,88));
    assert(!n64_fx_record_span(32,127,32,88));
    assert(!n64_fx_record_span(32,128,33,88));
    assert(!n64_fx_record_span(32,128,0xfffffff0u,88));
    return 0;
}
