#pragma once

#include "Common.h"

#include <sys/socket.h>

class Server
{
/* Member */
public:
    char buff[BuffLength];
    int nbytes;
    struct sockaddr_in client_addr;

private:
    const int port = 4347;
    const char ip[IPLength] = "0.0.0.0";

    struct sockaddr_in server_addr;
    int server_fd;

/* Function */
public:
    Server();
    ~Server();

    int Initialize();

    int Send();
    int Recv();
private:

};
