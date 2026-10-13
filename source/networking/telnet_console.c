/*
TELNET_CONSOLE.C

symbols in this file:
001201D0 00e0:
	_telnet_console_initialize (0000)
001202B0 0050:
	_telnet_console_dispose (0000)
00120300 00c0:
	_telnet_console_print (0000)
001203C0 01f0:
	_code_001203c0 (0000)
001205B0 0160:
	_telnet_console_process (0000)
00288D04 003e:
	??_C@_0DO@PCBIDEOI@create_transport_endpoint?$CI?$CJ?5fail@ (0000)
00288D44 0032:
	??_C@_0DC@CJAJBFKG@bind_endpoint?$CI?$CJ?5failed?5on?5telnet@ (0000)
00288D78 0034:
	??_C@_0DE@MBCCFOEL@listen_endpoint?$CI?$CJ?5failed?5on?5teln@ (0000)
00288DAC 0021:
	??_C@_0CB@NCDEMBCM@connection?5lost?5to?5telnet?5client@ (0000)
00288DD0 002f:
	??_C@_0CP@LBGDAMPB@?$AN?6overflowed?5client?5buffer?$DL?5rese@ (0000)
00288E00 0028:
	??_C@_0CI@BALEJICP@failed?5to?5write?5to?5telnet?5client@ (0000)
00288E28 000d:
	??_C@_0N@FIIHEHGK@?$AN?6goodbye?$CB?$AN?6?$AA@ (0000)
00288E38 0028:
	??_C@_0CI@BMDHLBAG@connection?5lost?5to?5telnet?5client@ (0000)
00288E60 001f:
	??_C@_0BP@BBNCCABM@error?5processing?5telnet?5client?$AA@ (0000)
00288E80 0048:
	??_C@_0EI@HONFGAEA@sorry?5?9?5the?5maximum?5number?5of?5cl@ (0000)
00288EC8 0021:
	??_C@_0CB@MOGHMNHN@Would?5you?5like?5to?5play?5a?5game?$DP?$AN?6@ (0000)
00456D00 008c:
	_telnet_console_globals (0000)
*/


/* ---------- headers */

#include "cseries.h"
#include "bungie_net/network/transport.h"
#include "bungie_net/network/transport_endpoint.h"
#include "hs/hs.h"
#include "networking/telnet_console.h"

/* ---------- constants */

enum
{
	MAXIMUM_TELNET_CLIENTS = 1,
	TELNET_CLIENT_BUFFER_SIZE = 128,
	TELNET_CONSOLE_PORT = 23,
	_transport_endpoint_type_telnet = 0x12
};

/* ---------- macros */

/* ---------- structures */

struct telnet_client
{
	struct transport_endpoint *endpoint;
	char buffer[TELNET_CLIENT_BUFFER_SIZE];
};

struct telnet_console_globals
{
	struct transport_endpoint *listening_endpoint;
	struct telnet_client clients[MAXIMUM_TELNET_CLIENTS];
	boolean initialized;
};

/* ---------- prototypes */

static boolean process_telnet_client_buffer(
	char *buffer,
	long size,
	struct telnet_client *client);

/* ---------- globals */

struct telnet_console_globals telnet_console_globals = {0};

/* ---------- public code */

void telnet_console_initialize(
	void)
{
	csmemset(&telnet_console_globals, 0, sizeof(telnet_console_globals));

	telnet_console_globals.listening_endpoint = create_transport_endpoint(_transport_endpoint_type_telnet);
	if (telnet_console_globals.listening_endpoint)
	{
		struct transport_address address = {{0}};

		address.address_length = IPV4_ADDRESS_LENGTH;
		address.port = TELNET_CONSOLE_PORT;

		if (bind_endpoint(telnet_console_globals.listening_endpoint, &address)==_transport_error_none)
		{
			if (listen_endpoint(telnet_console_globals.listening_endpoint)==_transport_error_none)
			{
				telnet_console_globals.initialized = TRUE;
			}
			else
			{
				error(2, "listen_endpoint() failed on telnet console endpoint");
				delete_transport_endpoint(telnet_console_globals.listening_endpoint);
				telnet_console_globals.listening_endpoint = NULL;
			}
		}
		else
		{
			error(2, "bind_endpoint() failed on telnet console endpoint");
			delete_transport_endpoint(telnet_console_globals.listening_endpoint);
			telnet_console_globals.listening_endpoint = NULL;
		}
	}
	else
	{
		error(2, "create_transport_endpoint() failed on telnet console endpoint");
	}

	return;
}

void telnet_console_dispose(
	void)
{
	if (telnet_console_globals.initialized)
	{
		if (telnet_console_globals.listening_endpoint)
			delete_transport_endpoint(telnet_console_globals.listening_endpoint);
		if (telnet_console_globals.clients[0].endpoint)
			delete_transport_endpoint(telnet_console_globals.clients[0].endpoint);
	}

	csmemset(&telnet_console_globals, 0, sizeof(telnet_console_globals));

	return;
}

void telnet_console_print(
	char *string)
{
	if (telnet_console_globals.initialized && string && string[0])
	{
		long length = csstrlen(string);

		if (telnet_console_globals.clients[0].endpoint)
		{
			long result = write_endpoint(telnet_console_globals.clients[0].endpoint, "\r\n", 2);

			if (result>0)
			{
				result = write_endpoint(telnet_console_globals.clients[0].endpoint, string, length);
				if (result>0)
				{
					if (telnet_console_globals.clients[0].buffer[0])
					{
						result = write_endpoint(
							telnet_console_globals.clients[0].endpoint,
							telnet_console_globals.clients[0].buffer,
							csstrlen(telnet_console_globals.clients[0].buffer));
					}

					if (result>0)
						return;
				}
			}

			error(2, "connection lost to telnet client");
			delete_transport_endpoint(telnet_console_globals.clients[0].endpoint);
			telnet_console_globals.clients[0].endpoint = NULL;
		}
	}

	return;
}

void telnet_console_process(
	void)
{
	if (telnet_console_globals.initialized)
	{
		char buffer[32];
		long count;

		if (endpoint_readable(telnet_console_globals.listening_endpoint, 0))
		{
			struct transport_endpoint *endpoint = accept_endpoint(telnet_console_globals.listening_endpoint);

			if (endpoint)
			{
				long client_index;

				for (client_index = 0; client_index<MAXIMUM_TELNET_CLIENTS; client_index++)
				{
					if (!telnet_console_globals.clients[client_index].endpoint)
					{
						if (write_endpoint(
							endpoint,
							"Would you like to play a game?\r\n",
							csstrlen("Would you like to play a game?\r\n"))<=0)
						{
							delete_transport_endpoint(endpoint);
						}
						else
						{
							telnet_console_globals.clients[client_index].endpoint = endpoint;
							telnet_console_globals.clients[client_index].buffer[0] = 0;
						}

						break;
					}
				}

				if (client_index==MAXIMUM_TELNET_CLIENTS)
				{
					write_endpoint(
						endpoint,
						"sorry - the maximum number of clients are already connected. goodbye!\r\n",
						csstrlen("sorry - the maximum number of clients are already connected. goodbye!\r\n"));
					delete_transport_endpoint(endpoint);
				}
			}
		}

		if (telnet_console_globals.clients[0].endpoint &&
			endpoint_readable(telnet_console_globals.clients[0].endpoint, 0))
		{
			count = read_endpoint(telnet_console_globals.clients[0].endpoint, buffer, sizeof(buffer));
			if (count>0)
			{
				if (process_telnet_client_buffer(buffer, count, telnet_console_globals.clients))
					return;

				error(2, "error processing telnet client");
			}
			else
			{
				error(2, "connection lost to telnet client ('%s')", transport_error_to_string((short)count));
			}

			if (telnet_console_globals.clients[0].endpoint)
			{
				delete_transport_endpoint(telnet_console_globals.clients[0].endpoint);
				telnet_console_globals.clients[0].endpoint = NULL;
			}
		}
	}

	return;
}

/* ---------- private code */

static boolean process_telnet_client_buffer(
	char *buffer,
	long size,
	struct telnet_client *client)
{
	boolean result = TRUE;
	long index;

	for (index = 0; result && index<size; index++)
	{
		char *character = buffer+index;
		long length;

		if (*character>0x7f)
			continue;

		if (isalnum(*character) || ispunct(*character) || *character==' ')
		{
			length = csstrlen(client->buffer)+1;
			if (length>=TELNET_CLIENT_BUFFER_SIZE)
			{
				client->buffer[0] = 0;
				length = write_endpoint(
					client->endpoint,
					"\r\noverflowed client buffer; resetting buffer\r\n",
					csstrlen("\r\noverflowed client buffer; resetting buffer\r\n"));
				if (length<=0)
				{
					error(2, "failed to write to telnet client ('%s')",
						transport_error_to_string((short)length));
					return FALSE;
				}

				return result;
			}

			client->buffer[length-1] = *character;
			client->buffer[length] = 0;
		}
		else
		{
			switch (*character)
			{
			case 10:
			case 13:
				if (client->buffer[0])
				{
					char expression[TELNET_CLIENT_BUFFER_SIZE];

					csstrncpy(expression, client->buffer, TELNET_CLIENT_BUFFER_SIZE-1);
					expression[TELNET_CLIENT_BUFFER_SIZE-1] = 0;
					client->buffer[0] = 0;

					if (hs_compile_and_evaluate(expression))
					{
						if (write_endpoint(client->endpoint, "\r\n", 2)<=0)
							result = FALSE;
					}
				}
				continue;

			case 8:
				if (client->buffer[0])
				{
					length = csstrlen(client->buffer);
					if (length>0)
						client->buffer[length-1] = 0;
				}
				break;

			case 4:
				write_endpoint(
					client->endpoint,
					"\r\ngoodbye!\r\n",
					csstrlen("\r\ngoodbye!\r\n"));
				delete_transport_endpoint(client->endpoint);
				client->endpoint = NULL;
				index = size;
				continue;

			default:
				continue;
			}
		}

		length = write_endpoint(client->endpoint, character, 1);
		if (length<=0)
		{
			error(2, "failed to write to telnet client ('%s')",
				transport_error_to_string((short)length));
			result = FALSE;
		}
	}

	return result;
}
