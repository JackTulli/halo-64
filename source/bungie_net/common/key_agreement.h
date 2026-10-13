/*
KEY_AGREEMENT.H
*/

#ifndef __KEY_AGREEMENT_H
#define __KEY_AGREEMENT_H
#pragma once

/* ---------- headers */

#include "cseries.h"

/* ---------- structures */

struct public_key;
struct transport_endpoint;

/* ---------- prototypes/KEY_AGREEMENT.C */

long is_message_encryption_key_message(
	word const *msgptr,
	word message_size,
	byte *packet_type);
boolean initiate_key_exchange(
	struct transport_endpoint *endpoint,
	struct public_key *key,
	struct public_key *prime,
	struct public_key *secret);
boolean complete_key_exchange(
	struct transport_endpoint *endpoint,
	word const *msgptr,
	struct public_key const *prime,
	struct public_key *secret,
	struct public_key *private_key);

void initialize_key_agreement_packets(
	void);

#endif // __KEY_AGREEMENT_H
