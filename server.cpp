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
    server_fd = socket(AF_INET, SOCK_DGRAM, 0);  //创建网络通信对象
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip);

    //绑定socket对象与通信链接
    int ret = bind(server_fd, (struct sockaddr *)&addr, sizeof(addr));
    if (ret < 0)
    {
        cerr << "Error: bind" << endl;
        return Error;
    }
    
    cout << "----------Server Start Running----------" << endl;
    return OK;
}

int Server::Recv()
{
    int res = 0;
    socklen_t len = sizeof(client_addr);
    if(res = recvfrom(server_fd, buff, BuffLength, 0, (struct sockaddr*)&client_addr, &len) < 0)
    {
        //cerr << "Failed to receive from client" << endl;
        return ERROR;
    }
    return res;
}
int Server::Send(char *buff, struct sockaddr_in client_addr, int count)
{
    int res = 0;
    if(res = sendto(server_fd, buff, BuffLength, 0, (struct sockaddr*)&client_addr, sizeof(struct sockaddr)) < 0)
    {
       // cerr << "Failed to send to " << inet_ntoa(client_addr.sin_addr.s_addr) << "[" << ntohs(client_addr.port) << "]" << endl;
        return ERROR;
    }
    return res;
}

int Server::MainActivity()
{
    //just do once
    socklen_t len = sizeof(client_addr);
    int res = recvfrom(server_fd, buff, BuffLength, MSG_DONTWAIT, (struct sockaddr *)&client_addr, &len);
    if (res <= 0)
        return NoPack;
    else
        return dealpack(this, res, client_addr, buff);
}

/* private */
