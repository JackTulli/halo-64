/*
NETWORK_CONNECTION.H

header included in hcex build.
*/

#ifndef __NETWORK_CONNECTION_H
#define __NETWORK_CONNECTION_H
#pragma once

/* ---------- constants */

enum
{
#ifdef HALO_LINUX
	/* the distributed netcode's per-tick state (port/linux/NETCODE.md): as
	large as fits one Ethernet frame with the internet play tunnel's
	overhead (every native build must agree) */
	DATAGRAM_MAXIMUM_SIZE = 1200,
#else
	DATAGRAM_MAXIMUM_SIZE = 400,
#endif
	MAXIMUM_NUMBER_OF_LOCAL_PLAYERS = 4,
	_connection_dont_timeout = 0,
	_connection_create_server_bit = 0,
	_connection_create_clientside_client_bit,
	_connection_create_serverside_client_bit,
};

/* ---------- macros */

/* ---------- structures */

struct network_connection;
struct transport_address;
struct transport_endpoint;

typedef void (*network_connection_rejection_procedure)(
	struct transport_endpoint *endpoint);

/* ---------- prototypes/NETWORK_CONNECTION.C */

void network_connection_initialize(
	void);
boolean network_connection_connected(
	struct network_connection *connection);
void network_connection_get_address(
	struct network_connection *connection,
	struct transport_address *reliable_address,
	struct transport_address *unreliable_address);
boolean network_connection_connect(
	struct network_connection *connection,
	struct transport_address const *remote_address,
	void *process_reference);
struct network_connection *network_connection_new(
	unsigned long flags,
	word well_known_port);
boolean network_connection_read(
	struct network_connection *connection,
	void *buffer,
	word *buffer_size,
	struct transport_address *source_address);
boolean network_connection_write(
	struct network_connection *connection,
	void *message,
	word buffer_size,
	struct transport_address *dest_address,
	boolean reliable);
boolean network_connection_idle(
	struct network_connection *connection,
	long timeout,
	struct network_connection **new_client_connection);
boolean network_connection_disconnect(
	struct network_connection *connection);
boolean network_server_close_client_connection(
	struct network_connection *server_connection,
	struct network_connection *client_connection);
void network_connection_delete(
	struct network_connection *connection);
void network_connection_set_connection_rejection_procedure(
	struct network_connection *connection,
	network_connection_rejection_procedure connection_rejection_procedure);
boolean network_connection_active(
	struct network_connection *connection);
boolean network_connection_going_stale(
	struct network_connection *connection);
void network_connection_keep_alive(
	struct network_connection *connection);
boolean network_connection_server_accept_client_connection(
	struct network_connection *server_connection,
	struct network_connection *client_connection);
void network_server_allow_client_connections(
	struct network_connection *server_connection,
	boolean allow_client_connections);

/* ---------- globals */

extern boolean global_connection_dont_timeout;

/* ---------- public code */

#endif // __NETWORK_CONNECTION_H
