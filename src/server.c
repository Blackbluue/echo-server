#include <common/netcode.h>
#include <stdint.h>
#include <stdio.h>

int
demo_service(const struct request *req)
{
    printf("Received request from %s:%d\n", req->client_addr, req->client_port);
    printf("Request data: %s\n", req->data);
    return 0;
}

int
main(void)
{
    // TODO: accept server port from command line
    // TODO: accept max connections from command line
    // TODO: define service function
    uint16_t port      = 1025;
    uint32_t max_conns = 0;
    service  s_handler = demo_service;
    server  *server    = server_create(s_handler, port, max_conns);
    if (!server)
    {
        fprintf(stderr, "Failed to create server\n");
        return 1;
    }

    // run service until server is shutdown
    if (server_run(server))
    {
        fprintf(stderr, "Failed to run server\n");
        server_destroy(server);
        return 1;
    }
    server_wait(server);
    server_destroy(server);
    return 0;
}
