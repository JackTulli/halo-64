/*
TRANSPORT_ENDPOINT.H
*/

#ifndef __TRANSPORT_ENDPOINT_H
#define __TRANSPORT_ENDPOINT_H
#pragma once

/* ---------- headers */

#include "cseries/cseries.h"

/* ---------- constants */

enum
{
	_transport_endpoint_connected_bit = 0,
	_transport_endpoint_listening_bit,
	_transport_endpoint_readable_bit,
	_transport_endpoint_in_set_bit,
	_transport_endpoint_nonblocking_bit,
	_transport_endpoint_client_bit
};

/* ---------- structures */

struct transport_endpoint
{
	long socket;
	char flags;
	char type;
	short error;
};

struct transport_endpoint_set;
struct transport_address;
struct connect_process_input;

typedef struct connect_process_input *transport_connect_process_ref;

/* ---------- prototypes/TRANSPORT_ENDPOINT_WINSOCK.C, TRANSPORT_ENDPOINT_SET_WINSOCK.C */

struct transport_endpoint *create_transport_endpoint(
	long type);
struct transport_endpoint_set *create_endpoint_set(
	short maximum_endpoints);
long read_endpoint(
	struct transport_endpoint *endpoint,
	void *buffer,
	long size);
long read_from_endpoint(
	struct transport_endpoint *endpoint,
	void *buffer,
	long size,
	struct transport_address *source_address);
long write_endpoint(
	struct transport_endpoint *endpoint,
	void const *buffer,
	long size);
long write_to_endpoint(
	struct transport_endpoint *endpoint,
	void const *buffer,
	long length,
	struct transport_address const *destination_address);
boolean endpoint_readable(
	struct transport_endpoint *endpoint,
	word timeout);
long endpoint_connected(
	struct transport_endpoint const *endpoint);
long endpoint_listening(
	struct transport_endpoint const *endpoint);
boolean endpoint_blocking(
	struct transport_endpoint const *endpoint);
short get_endpoint_address(
	struct transport_endpoint *endpoint,
	struct transport_address *address);
short connect_endpoint(
	struct transport_endpoint *endpoint,
	struct transport_address const *address);
short connect_endpoint_async(
	struct transport_endpoint *endpoint,
	struct transport_address const *address,
	transport_connect_process_ref *process_reference);
void cancel_connect_process(
	transport_connect_process_ref input);
short set_endpoint_blocking(
	struct transport_endpoint *endpoint,
	long blocking);
short bind_endpoint(
	struct transport_endpoint *endpoint,
	struct transport_address *address);
short listen_endpoint(
	struct transport_endpoint *endpoint);
struct transport_endpoint *accept_endpoint(
	struct transport_endpoint *listening_endpoint);
short reject_endpoint(
	struct transport_endpoint *listening_endpoint);
void disconnect_endpoint(
	struct transport_endpoint *endpoint);
short poll_endpoint_set(
	struct transport_endpoint_set *set,
	word timeout);
void rewind_endpoint_set(
	struct transport_endpoint_set *set);
struct transport_endpoint *get_next_endpoint_from_set(
	struct transport_endpoint_set *set);
long count_endpoints_in_set(
	struct transport_endpoint_set *set);
short add_endpoint_to_set(
	struct transport_endpoint *endpoint,
	struct transport_endpoint_set *set);
short remove_endpoint_from_set(
	struct transport_endpoint *endpoint,
	struct transport_endpoint_set *set);
short delete_endpoint_set(
	struct transport_endpoint_set *set);
void delete_transport_endpoint(
	struct transport_endpoint *endpoint);
void transport_get_nonce(
	void *nonce,
	long nonce_length);
char const *winsock_error_to_string(
	long error);

#endif // __TRANSPORT_ENDPOINT_H
