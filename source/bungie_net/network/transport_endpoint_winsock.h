/*
TRANSPORT_ENDPOINT_WINSOCK.H

header included in hcex build.
*/

#ifndef __TRANSPORT_ENDPOINT_WINSOCK_H
#define __TRANSPORT_ENDPOINT_WINSOCK_H
#pragma once

/* ---------- headers */

#include <xtl.h>
#include "transport_endpoint.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

/* ---------- prototypes/TRANSPORT_ENDPOINT_SET_WINSOCK.C */

void transport_client_stop(
	void);
void transport_client_start(
	XNADDR const *xnaddr,
	XNKEY const *key,
	XNKID const *key_id,
	word port,
	struct transport_address *address);
short transport_server_initialize(
	void);
short transport_server_terminate(
	void);
XNADDR *transport_get_xnaddr(
	XNADDR *address);
XNKID transport_get_key_id(
	void);
XNKEY transport_get_key(
	void);

/* ---------- globals */

/* ---------- public code */

#endif // __TRANSPORT_ENDPOINT_WINSOCK_H
