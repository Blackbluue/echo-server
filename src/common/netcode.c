#include <common/netcode.h>
#include <poll.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

/* DATA */
enum
{
    LISTENER_SD             = 0,  // index of the listener socket
    BAD_FD                  = -1, // sentinel value for unused file descriptor
    INITIAL_CONNECTIONS     = 7,  // initial number of connections to allocate
    DEFAULT_MAX_CONNECTIONS = 64, // default maximum number of connections
};
enum error_codes
{
    SUCCESS = 0,
    FAILURE = -1,
};

struct server
{
    pthread_t           main_thread;
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
 * @brief Validate a port number.
 *
 * @param port The port number to validate.
 * @param port_str Where to store the port number as a string.
 * @param port_str_len The length of the port_str buffer.
 * @return int 0 on success, non-zero on failure.
 */
int
validate_port(uint16_t port, char *port_str, size_t port_str_len)
{
    if (port < 1024 || port > 65535)
    {
        fprintf(stderr, "Invalid port: %d\n", port);
        return FAILURE;
    }
    snprintf(port_str, port_str_len, "%d", port);
    return SUCCESS;
}

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

void
server_destroy(server *s)
{
    // destroying server, fail if it is running
    if (s)
    {
        if (s->listener_sd != BAD_FD)
        {
            close(s->listener_sd);
            s->listener_sd = BAD_FD;
        }
        for (size_t i = 1; i < s->sock_count; i++)
        {
            // make sure only close the socket if it was opened
            // close_connection(s->connections[i]);
            if (s->pollfds[i].fd != BAD_FD)
            {
                close(s->pollfds[i].fd);
            }
        }
        free(s->connections);
        free(s->pollfds);
        free(s);
    }
}

int
run_server(server *s)
{
    if (!s)
    {
        return FAILURE;
    }

    // start listening for connections
    int err = SUCCESS;
    // int err
    //     = pthread_create(&s->main_thread, NULL, manage_server_connections,
    //     s);
    if (err != SUCCESS)
    {
        perror("Could not create main thread");
    }
    return err;
}

void
server_wait(server *s)
{
    if (!s)
    {
        return;
    }
    // shutdown the server
    // close the socket to stop accepting connections
    // close all connections
}
