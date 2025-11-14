#include <arpa/inet.h>

typedef struct sockaddr_in socketaddr_t;

struct {
	int fd;
	socketaddr_t addr;
	socklen_t len;
} client;

void
client_setup()
{
	client.len = sizeof(client.addr);
}
