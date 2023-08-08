#ifndef NETCODE_H
#define NETCODE_H
#include <stdint.h>
#include <unistd.h>

/* DATA */
/* request structure that will be sent to the service on a client connection
 * attempt */
struct request
{
    size_t   data_len;
    uint8_t *data;
    uint8_t *client_addr;
    uint16_t client_port;
};

/* server structure that will be used to store the server's state */
typedef struct server server;
/* service function pointer that will be used to run the service on the server
 */
typedef int (*service)(const struct request *req);

/**
 * @brief Initialize a server.
 *
 * @param service_handler The service to run on the server.
 * @param port The port to listen on.
 * @param max_connections The maximum number of connections to accept.
 * @return server* A pointer to the server on success, NULL on failure.
 */
server *server_create(service  service_handler,
                      uint16_t port,
                      uint32_t max_connections);

/**
 * @brief Destroy a server.
 *
 * @param s The server to destroy.
 */
void server_destroy(server *s);
#endif /* NETCODE_H */
