#pragma once

#include "Common.h"

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <iostream> // cin,cout
#include <iomanip>  // setw
#include <cstdlib>
#include <iostream>

using namespace std;

class Server
{
/* Member */
public:
    char buff[BuffLength];
    int nbytes;
    struct sockaddr_in client_addr;

    int (*callback)(Server *server, int nbytes, struct sockaddr_in client_addr, char *buff);

private:
    const int port = 19434;
    const char ip[IPLength] = "0.0.0.0";

    struct sockaddr_in server_addr;
    int server_fd;

/* Function */
public:
    Server();
    ~Server();

    int Initialize();

    int MainActivity();

    int Recv();
    int Send(char *buff, struct sockaddr_in client_addr, int count);

    int (*dealpack)(class Server* s, int nbytes, struct sockaddr_in sock_addr, char* buff);
private:

};
