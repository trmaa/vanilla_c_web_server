#include <arpa/inet.h>
#include "client.h"

typedef struct sockaddr_in socketaddr_t;

void
client_setup(client_t* client)
{
	client->len = sizeof(client->addr);
}
