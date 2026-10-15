/* Transient menu sprites/fonts share the unused tail of the map arena.
 * The menu releases them after rspq_wait(), before any map can load there. */
#include <libdragon.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "n64.h"

struct block { unsigned long size; int free; struct block *next; unsigned long pad; };
static uint8_t *start, *end;
static struct block *first;

void n64_menu_memory_begin(void)
{
    unsigned long size;
    uint8_t *home = n64_map_home_free_part(&size);
    /* Two 640x480 menu buffers plus their Z buffer, with alignment guards. */
    unsigned long used = 3 * (640 * 480 * 2 + 64) + 512;
    start = end = NULL; first = NULL;
    if (!home || size <= used + sizeof(struct block)) return;
    start = home + used;
    end = home + size;
    first = (struct block *)start;
    first->size = end - start - sizeof(*first);
    first->free = 1;
    first->next = NULL;
}

void *n64_menu_alloc(unsigned long size)
{
    size = (size + 15) & ~15UL;
    for (struct block *b = first; b; b = b->next)
    {
        if (!b->free || b->size < size) continue;
        if (b->size >= size + sizeof(*b) + 16)
        {
            struct block *next = (struct block *)((uint8_t *)(b + 1) + size);
            next->size = b->size - size - sizeof(*b);
            next->free = 1; next->next = b->next;
            b->size = size; b->next = next;
        }
        b->free = 0;
        return b + 1;
    }
    return malloc(size);
}

static int in_arena(const void *p)
{
    return start && (const uint8_t *)p >= start && (const uint8_t *)p < end;
}

void n64_menu_free(void *p)
{
    if (!p) return;
    if (!in_arena(p)) { free(p); return; }
    ((struct block *)p - 1)->free = 1;
    for (struct block *b = first; b && b->next; )
    {
        if (b->free && b->next->free)
        {
            b->size += sizeof(*b) + b->next->size;
            b->next = b->next->next;
        }
        else b = b->next;
    }
}

void *n64_menu_sprite_load(const char *path)
{
    if (!first) return sprite_load(path);
    FILE *f = fopen(path, "rb");
    if (!f) return NULL;
    fseek(f, 0, SEEK_END);
    int file_size = ftell(f), size = file_size, needed = 0;
    rewind(f);
    asset_loadf_into(f, &size, NULL, &needed);
    void *buffer = n64_menu_alloc(needed);
    if (!buffer) { fclose(f); return NULL; }
    rewind(f); size = file_size;
    if (!asset_loadf_into(f, &size, buffer, &needed))
    { n64_menu_free(buffer); fclose(f); return NULL; }
    fclose(f);
    sprite_t *sprite = sprite_load_buf(buffer, size);
    assertf((void *)sprite == buffer, "Menu assets must use ordinary sprites");
    return sprite;
}

void n64_menu_sprite_free(void *p)
{
    if (!p) return;
    /* sprite_load_buf does not take ownership of its caller's buffer. */
    ((sprite_t *)p)->flags &= ~SPRITE_FLAGS_OWNEDBUFFER;
    sprite_free((sprite_t *)p);
    n64_menu_free(p);
}

void n64_menu_memory_end(void)
{
    for (struct block *b = first; b; b = b->next)
        assertf(b->free, "A menu asset survived into gameplay");
    first = NULL; start = end = NULL;
}
