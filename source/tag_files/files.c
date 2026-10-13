/*
FILES.C
*/

/* ---------- headers */

#include "cseries.h"
#include "files.h"

/* ---------- globals */

char file_location_volume_names[NUMBER_OF_FILE_REFERENCE_LOCATIONS][256];

/* ---------- public code */

void file_location_set_volume(
	short location,
	const char *volume_name)
{
	match_assert("c:\\halo\\SOURCE\\tag_files\\files.c", 75, location>0 && location<NUMBER_OF_FILE_REFERENCE_LOCATIONS);
	match_assert("c:\\halo\\SOURCE\\tag_files\\files.c", 76, strlen(file_location_volume_names[location])==0);
	match_assert("c:\\halo\\SOURCE\\tag_files\\files.c", 77, strlen(volume_name)<=MAXIMUM_FILENAME_LENGTH);

	strncpy(file_location_volume_names[location], volume_name, NUMBEROF(*file_location_volume_names)-1);
	file_location_volume_names[location][NUMBEROF(*file_location_volume_names)-1] = '\0';

	return;
}

struct file_reference *file_reference_create(
	struct file_reference *reference,
	short location)
{
	struct file_reference_info *info = (struct file_reference_info *)reference;
	match_assert("c:\\halo\\SOURCE\\tag_files\\files.c", 91, info);
	match_assert("c:\\halo\\SOURCE\\tag_files\\files.c", 92, location>=NONE && location<NUMBER_OF_FILE_REFERENCE_LOCATIONS);

	/* probably a macro */
	memset(info, 0, FILE_REFERENCE_SIZE);
	info->signature = FILE_REFERENCE_SIGNATURE;
	info->location = location;

	return reference;
}

long find_files(
	unsigned long flags,
	const struct file_reference *directory,
	long maximum_count,
	struct file_reference *references)
{
	long file_count = 0;

	match_assert("c:\\halo\\SOURCE\\tag_files\\files.c", 257, maximum_count>0);
	match_assert("c:\\halo\\SOURCE\\tag_files\\files.c", 258, references);

	find_files_start(flags, directory);

	for (file_count; file_count<maximum_count; file_count++, references++)
	{
		if (!find_files_next(references, NULL))
		{
			break;
		}
	}

	return file_count;
}

void *file_read_into_memory(
	struct file_reference *reference,
	unsigned long *size)
{
	void *buffer = NULL;

	if (file_open(reference, FLAG(_permission_read_bit)))
	{
		unsigned long eof = file_get_eof(reference);
		*size = eof;
		buffer = match_malloc("c:\\halo\\SOURCE\\tag_files\\files.c", 280, eof);

		if (buffer)
		{
			if (!file_read(reference, *size, buffer))
			{
				match_free("c:\\halo\\SOURCE\\tag_files\\files.c", 286, buffer);
				buffer = NULL;
			}
		}

		file_close(reference);
	}

	return buffer;
}

void file_printf(
	struct file_reference *file,
	char *format, ...)
{
	char buffer[1024];
	va_list arglist;

	va_start(arglist, format);

	if (format)
	{
		vsprintf(buffer, format, arglist);
		file_write(file, strlen(buffer), buffer);
		file_set_eof(file, file_get_position(file));
	}

	va_end(arglist);

	return;
}

struct file_reference_info *file_reference_get_info(
	struct file_reference *reference)
{
	struct file_reference_info *info = (struct file_reference_info *)reference;
	match_assert("c:\\halo\\SOURCE\\tag_files\\files.c", 508, info);
	match_assert("c:\\halo\\SOURCE\\tag_files\\files.c", 509, info->signature==FILE_REFERENCE_SIGNATURE);
	match_assert("c:\\halo\\SOURCE\\tag_files\\files.c", 510, VALID_FLAGS(info->flags, NUMBER_OF_REFERENCE_INFO_FLAGS));
	match_assert("c:\\halo\\SOURCE\\tag_files\\files.c", 511, info->location>=NONE && info->location<NUMBER_OF_FILE_REFERENCE_LOCATIONS);

	return info;
}

struct file_reference *file_reference_copy(
	struct file_reference *destination,
	const struct file_reference *source)
{
	file_reference_get_info((struct file_reference *)source);
	memcpy(destination, source, sizeof(struct file_reference_info));

	return destination;
}

struct file_reference *file_reference_add_directory(
	struct file_reference *reference,
	const char *directory)
{
	struct file_reference_info *info = file_reference_get_info(reference);
	match_assert("c:\\halo\\SOURCE\\tag_files\\files.c", 137, directory);
	match_assert("c:\\halo\\SOURCE\\tag_files\\files.c", 138, !TEST_FLAG(info->flags, _has_filename_bit));

	file_path_add_name(info->path, directory);

	return reference;
}

struct file_reference *file_reference_set_name(
	struct file_reference *reference,
	const char *name)
{
	struct file_reference_info *info = file_reference_get_info(reference);
	match_assert("c:\\halo\\SOURCE\\tag_files\\files.c", 151, name);

	if (TEST_FLAG(info->flags, _has_filename_bit))
	{
		file_path_remove_name(info->path);
	}

	file_path_add_name(info->path, name);
	SET_FLAG(info->flags, _has_filename_bit, TRUE);

	return reference;
}

short file_reference_get_location(
	const struct file_reference *reference)
{
	struct file_reference_info *info = file_reference_get_info((struct file_reference *)reference);
	return info->location;
}

char *file_reference_get_name(
	const struct file_reference *reference,
	unsigned long flags,
	char *name)
{
	struct file_reference_info *info = file_reference_get_info((struct file_reference *)reference);

	char full_path[256] = { 0 };
	char *filename;
	char *directory;
	char *parent_directory;
	char *extension;

	match_assert("c:\\halo\\SOURCE\\tag_files\\files.c", 185, name);
	match_assert("c:\\halo\\SOURCE\\tag_files\\files.c", 186, VALID_FLAGS(info->flags, NUMBER_OF_NAME_FLAGS));
	match_assert("c:\\halo\\SOURCE\\tag_files\\files.c", 187, flags);
	match_assert("c:\\halo\\SOURCE\\tag_files\\files.c", 188, flags!=(FLAG(_name_directory_bit)|FLAG(_name_extension_bit)));
	match_assert("c:\\halo\\SOURCE\\tag_files\\files.c", 189, !TEST_FLAG(flags, _name_directory_bit) || !TEST_FLAG(flags, _name_parent_directory_bit));

	file_location_get_full_path(info->location, info->path, full_path);
	file_path_split(full_path, &directory, &parent_directory, &filename, &extension, TEST_FLAG(info->flags, _has_filename_bit));

	name[0] = '\0';

	if (TEST_FLAG(flags, _name_directory_bit))
	{
		file_path_add_name(name, directory);
	}

	if (TEST_FLAG(flags, _name_parent_directory_bit))
	{
		file_path_add_name(name, parent_directory);
	}

	if (TEST_FLAG(flags, _name_filename_bit))
	{
		file_path_add_name(name, filename);
	}

	if (TEST_FLAG(flags, _name_extension_bit))
	{
		file_path_add_extension(name, extension);
	}

	return name;
}

boolean file_references_equal(
	const struct file_reference *reference0,
	const struct file_reference *reference1)
{
	struct file_reference_info *info1 = file_reference_get_info((struct file_reference *)reference0);
	struct file_reference_info *info2 = file_reference_get_info((struct file_reference *)reference1);
	boolean equal = FALSE;

	if (info1->location==info2->location && !strcmp(info1->path, info2->path))
	{
		equal = TRUE;
	}

	return equal;
}

struct file_reference *file_reference_create_from_path(
	struct file_reference *reference,
	const char *path,
	boolean is_directory)
{
	struct file_reference_info *info = (struct file_reference_info *)reference;
	match_assert("c:\\halo\\SOURCE\\tag_files\\files.c", 91, info);

	memset(info, 0, FILE_REFERENCE_SIZE);
	info->signature = FILE_REFERENCE_SIGNATURE;
	info->location = NONE;

	if (is_directory)
	{
		file_reference_add_directory(reference, path);
	}
	else
	{
		file_reference_set_name(reference, path);
	}

	return reference;
}

void directory_create_or_delete_contents(
	const char *directory_name)
{
	struct file_reference directory;
	struct file_reference file;
	struct file_reference_info *info = (struct file_reference_info *)&directory;

	memset(info, 0, FILE_REFERENCE_SIZE);
	info->signature = FILE_REFERENCE_SIGNATURE;
	info->location = NONE;

	file_reference_add_directory(&directory, directory_name);

	if (file_exists(&directory))
	{
		find_files_start(0, &directory);
		while (find_files_next(&file, NULL))
		{
			file_delete(&file);
		}
	}
	else
	{
		file_create(&directory);
	}

	return;
}

boolean datastore_read(
	const char *file_name,
	const char *field_name,
	long length,
	void *data)
{
	struct file_reference file_ref;
	boolean success;
	unsigned long datastore_size = 0;
	struct datastore *datastore;

	struct file_reference_info *info = (struct file_reference_info *)&file_ref;

	match_assert("c:\\halo\\SOURCE\\tag_files\\files.c", 369, NULL != file_name);
	match_assert("c:\\halo\\SOURCE\\tag_files\\files.c", 370, NULL != field_name);
	match_assert("c:\\halo\\SOURCE\\tag_files\\files.c", 371, '\0' != file_name[0]);
	match_assert("c:\\halo\\SOURCE\\tag_files\\files.c", 372, '\0' != field_name[0]);
	match_assert("c:\\halo\\SOURCE\\tag_files\\files.c", 373, length < DATASTORE_MAX_DATA_SIZE);
	match_assert("c:\\halo\\SOURCE\\tag_files\\files.c", 374, strlen(field_name) < DATASTORE_MAX_FIELD_NAME_SIZE);

	memset(info, 0, FILE_REFERENCE_SIZE);
	info->signature = FILE_REFERENCE_SIGNATURE;
	info->location = NONE;

	file_reference_set_name(&file_ref, file_name);

	if (file_exists(&file_ref))
	{
		long entry_index;

		datastore = file_read_into_memory(&file_ref, &datastore_size);

		if (!datastore)
		{
			file_delete(&file_ref);
		}

		if (datastore_size!=sizeof(struct datastore))
		{
			match_free("c:\\halo\\SOURCE\\tag_files\\files.c", 389, datastore);
			file_delete(&file_ref);
			datastore = NULL;
		}

		if (datastore)
		{
			for (entry_index = 0; entry_index < NUMBER_OF_DATASTORE_ENTRIES; entry_index++)
			{
				if (!strcmp(datastore->entry[entry_index].name, field_name))
				{
					memcpy(data, datastore->entry[entry_index].data, length);
					success = TRUE;
					break;
				}

				if (datastore->entry[entry_index].name[0] == '\0')
				{
					break;
				}
			}

			match_free("c:\\halo\\SOURCE\\tag_files\\files.c", 416, datastore);
		}
	}

	/* BUG (preserved for exact matching): success is never initialised, so a missing
	 * file, a wrong-sized datastore or an absent field returns it unassigned (January
	 * 0x589370 keeps it in the high byte of the field_name argument slot, [ebp+0xf],
	 * written only at +0x1f5 and read at +0x19b and +0x20c). Not reached in January:
	 * cachebeta.exe has no call, jump or address reference to this function. A corrected
	 * build should initialise success to FALSE. Source-policy approval pending
	 * (2026-09-27 audit). */
	return success;
}

boolean datastore_write(
	const char *file_name,
	const char *field_name,
	long length,
	const void *data)
{
	struct file_reference file_ref;
	boolean success = FALSE;
	unsigned long datastore_size = 0;
	struct datastore *datastore;
	struct file_reference_info *info = (struct file_reference_info *)&file_ref;

	match_assert("c:\\halo\\SOURCE\\tag_files\\files.c", 430, NULL != file_name);
	match_assert("c:\\halo\\SOURCE\\tag_files\\files.c", 431, NULL != field_name);
	match_assert("c:\\halo\\SOURCE\\tag_files\\files.c", 432, '\0' != file_name[0]);
	match_assert("c:\\halo\\SOURCE\\tag_files\\files.c", 433, '\0' != field_name[0]);
	match_assert("c:\\halo\\SOURCE\\tag_files\\files.c", 434, length < DATASTORE_MAX_DATA_SIZE);
	match_assert("c:\\halo\\SOURCE\\tag_files\\files.c", 435, strlen(field_name) < DATASTORE_MAX_FIELD_NAME_SIZE);

	memset(info, 0, FILE_REFERENCE_SIZE);
	info->signature = FILE_REFERENCE_SIGNATURE;
	info->location = NONE;

	file_reference_set_name(&file_ref, file_name);

	if (file_exists(&file_ref))
	{
		datastore = file_read_into_memory(&file_ref, &datastore_size);

		if (!datastore)
		{
			file_delete(&file_ref);
		}

		if (datastore_size != sizeof(struct datastore))
		{
			match_free("c:\\halo\\SOURCE\\tag_files\\files.c", 450, datastore);
			file_delete(&file_ref);
			datastore = NULL;
		}
	}
	else
	{
		datastore = NULL;
	}

	if (!datastore)
	{
		datastore = match_malloc("c:\\halo\\SOURCE\\tag_files\\files.c", 461, sizeof(struct datastore));

		if (datastore)
		{
			memset(datastore, 0, sizeof(struct datastore));
		}
	}

	if (datastore)
	{
		long entry_index;

		for (entry_index = 0; entry_index < NUMBER_OF_DATASTORE_ENTRIES; entry_index++)
		{
			if (datastore->entry[entry_index].name[0] == '\0' ||
				!strcmp(datastore->entry[entry_index].name, field_name))
			{
				strcpy(datastore->entry[entry_index].name, field_name);
				memcpy(datastore->entry[entry_index].data, data, length);
				success = TRUE;
				break;
			}
		}

		if (!file_exists(&file_ref))
		{
			file_create(&file_ref);
		}

		if (file_open(&file_ref, FLAG(_permission_write_bit)))
		{
			file_write(&file_ref, sizeof(struct datastore), datastore);
			file_close(&file_ref);
		}

		match_free("c:\\halo\\SOURCE\\tag_files\\files.c", 492, datastore);
	}

	match_assert("c:\\halo\\SOURCE\\tag_files\\files.c", 495, success);

	return success;

}
