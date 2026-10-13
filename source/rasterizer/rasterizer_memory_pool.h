/*
RASTERIZER_MEMORY_POOL.H

Narrow cross-translation-unit interface owned by RASTERIZER_MEMORY_POOL.C.
*/

#ifndef __RASTERIZER_MEMORY_POOL_H
#define __RASTERIZER_MEMORY_POOL_H
#pragma once

const void *rasterizer_memory_alloc_const(
	const void *source,
	unsigned long size);

#endif /* __RASTERIZER_MEMORY_POOL_H */
