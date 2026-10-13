/*
CRC.H

header included in hcex build.
*/

#ifndef __CRC_H
#define __CRC_H
#pragma once

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

/* ---------- prototypes/CRC.C */

void crc_new(
	unsigned long *crc_reference);
void crc_checksum_buffer(
	unsigned long *crc_reference,
	void const *buffer,
	long buffer_size);

/* ---------- globals */

/* ---------- public code */

#endif // __CRC_H
