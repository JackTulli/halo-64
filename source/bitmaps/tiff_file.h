/*
TIFF_FILE.H
*/

#ifndef __TIFF_FILE_H
#define __TIFF_FILE_H
#pragma once

struct bitmap_data;
struct file_reference;

char const *tiff_export(
	struct file_reference *file,
	struct bitmap_data *bitmap);

#endif /* __TIFF_FILE_H */
