/*
NETWORK_CLIENT_MESSAGE_HANDLER.H

header included in hcex build.
*/

#ifndef __NETWORK_CLIENT_MESSAGE_HANDLER_H
#define __NETWORK_CLIENT_MESSAGE_HANDLER_H
#pragma once

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

struct network_game_client;
struct transport_address;

/* ---------- prototypes/NETWORK_CLIENT_MESSAGE_HANDLER.C */

boolean network_game_client_handle_message(
	struct network_game_client *client,
	word *message,
	short message_size,
	struct transport_address *source_address);

/* ---------- globals */

/* ---------- public code */

#endif // __NETWORK_CLIENT_MESSAGE_HANDLER_H
