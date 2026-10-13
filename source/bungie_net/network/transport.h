/*
TRANSPORT.H

header included in hcex build.
*/

#ifndef __TRANSPORT_H
#define __TRANSPORT_H
#pragma once

/* ---------- constants */

enum
{
	IPV4_ADDRESS_LENGTH = 4,
	IPV6_ADDRESS_LENGTH = 16,
	MAXIMUM_TRANSPORT_ADDRESS_LENGTH = 16,
};

enum transport_error
{
	_transport_result_connect_in_progress = -23,
	_transport_result_dns_lookup_in_progress,
	_transport_error_poll_error,
	_transport_error_endpoint_set_full,
	_transport_error_endpoint_not_in_set,
	_transport_error_options_failed,
	_transport_error_listen_failed,
	_transport_error_connect_failed,
	_transport_error_address_unknown,
	_transport_error_bind_endpoint,
	_transport_result_poll_timeout,
	_transport_error_bad_endpoint,
	_transport_error_buffers_full,
	_transport_error_seg_fault,
	_transport_error_out_of_memory,
	_transport_error_dns_lookup_failure,
	_transport_error_bad_input_parameters,
	_transport_result_already_initialized,
	_transport_error_not_initialized,
	_transport_result_operation_would_block,
	_transport_error_connection_lost,
	_transport_error_endpoint_io,
	_transport_error_unknown,
	_transport_error_none,
};

/* ---------- macros */

/* ---------- structures */

struct transport_address_data
{
	union
	{
		unsigned long ipv4_address;
		unsigned long long_words[MAXIMUM_TRANSPORT_ADDRESS_LENGTH / sizeof(unsigned long)];
		word words[MAXIMUM_TRANSPORT_ADDRESS_LENGTH / sizeof(word)];
		byte bytes[MAXIMUM_TRANSPORT_ADDRESS_LENGTH];
	};
};

struct transport_address
{
	struct transport_address_data address;
	word address_length;
	word port;
	long address_type;
};

/* ---------- prototypes */

short transport_initialize(
	void);
short transport_dispose(
	void);
struct transport_address *create_transport_address(
	struct transport_address_data const *address,
	word address_length,
	word port);
void delete_transport_address(
	struct transport_address *address);
long transport_address_equivalent(
	struct transport_address const *a,
	struct transport_address const *b);
char const *transport_address_to_string(
	struct transport_address const *addr);
char const *transport_error_to_string(
	short error);
void net_startup_debug(
	void);
boolean transport_is_nonce(
	void const *nonce,
	long nonce_length);
void transport_get_nonce(
	void *nonce,
	long nonce_length);
boolean transport_nonce_is_equal(
	void const *a,
	void const *b);
boolean transport_network_available(
	void);

/* ---------- globals */

extern boolean transport_initialized;

/* ---------- public code */

#endif // __TRANSPORT_H
