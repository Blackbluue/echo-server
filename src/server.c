#include <common/netcode.h>

int
main(void)
{
    server *s = server_create(NULL, 0, 0);
    server_destroy(s);
    return 0;
}
