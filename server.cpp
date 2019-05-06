#include "Server.h"

#include <iostream>

#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <dirent.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

using namespace std;

/* public */
Server::Server()
{
    // nothing to do
}
Server::~Server()
{
    // nothing to do
}

int Server::Initialize()
{
    if(server_fd = socket(AF_INET, SOCK_DGRAM, 0) < 0)
    {
        cerr << "Server failed to create fd" << endl;
        return ERROR;
    }

    int _REUSEADDR = 1;
    if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &_REUSEADDR, sizeof(_REUSEADDR)) == -1)
    {
        cerr << "Server failed to set REUSEADDR" << endl;
        return ERROR;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip);
    if(bind(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        cerr << "Server failed to bind" << endl;
        return ERROR;
    }

    return OK;
}

int Server::Send()
{
    int res = 0;
    if((res = sendto(server_fd, buff, BuffLength, 0, (struct addr*)client_addr, sizeof(struct sockaddr)) < 0)
    {
        cerr << "Failed to send to " << inet_ntoa(client_addr.sin_addr.s_addr) << "[" << ntohs(client_addr.port) << "]" << endl;
        return ERROR;
    }
    return OK;
}
int Server::Recv()
{
    int res = 0;
    if((res = recvfrom(server_fd, buff, BuffLength, 0, (struct addr*)&client_addr, sizeof(struct sockaddr)) < 0)
    {
        cerr << "Failed to receive from client" << endl;
        return ERROR;
    }
    return OK;
}

/* private */
