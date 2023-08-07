#include <common/netcode.h>
#include <poll.h>
#include <pthread.h>
#include <stdlib.h>

/* DATA */
enum
{
    LISTENER_SD             = 0,
    INITIAL_CONNECTIONS     = 7,
    DEFAULT_MAX_CONNECTIONS = 64,
};
struct server
{
    size_t              sock_count;      // number of sockets in use
    size_t              sock_cap;        // number of sockets allocated
    size_t              max_connections; // maximum number of connections
    service             service_handler;
    struct connection **connections;
    struct pollfd      *pollfds;
    int                 listener_sd;
    uint16_t            port;
};

/* PRIVATE FUNCTIONS */
/**
 * @brief Allocate a server struct.
 *
 * @param connection_capacity The number of connections to allocate space for.
 * @return server* A pointer to the allocated server struct.
 */
static server *
allocate_server(size_t connection_capacity)
{
    server *s = malloc(sizeof(*s));
    if (s == NULL)
    {
        return NULL;
    }
    // index 0 is the listener, with the rest being connections
    s->connections = calloc(connection_capacity, sizeof(*s->connections));
    if (s->connections == NULL)
    {
        free(s);
        return NULL;
    }
    s->connections[LISTENER_SD] = NULL;
    s->pollfds = calloc(connection_capacity, sizeof(*s->pollfds));
    if (s->pollfds == NULL)
    {
        free(s->connections);
        free(s);
        return NULL;
    }
    s->sock_count = 0;
    s->sock_cap   = connection_capacity;
    return s;
}

/* PUBLIC FUNCTIONS */
server *
server_create(service service_handler, uint16_t port, uint32_t max_connections)
{
    if (!port || !service_handler)
    {
        return NULL;
    }
    // allocation server with given max connections, set to
    // default if not given by user
    if (max_connections == 0)
    {
        max_connections = DEFAULT_MAX_CONNECTIONS;
    }
    size_t initial_cap = max_connections <= INITIAL_CONNECTIONS
                             ? max_connections
                             : INITIAL_CONNECTIONS;
    // +1 to max connections to account for listener
    server *s = allocate_server(initial_cap + 1);
    if (!s)
    {
        return NULL;
    }
    return s;
}
