#pragma once

#include "Common.h"

#include <string>
#include <cstring>

struct UserInfo
{
    char ip[IPLength];
    int port;
    int roomid;
    int side;
    char A[ChessSize * ChessSize];
    int plane;
    
    UserInfo()
    {
        ip[0] = '\0';
        port = NoConnect; roomid = NoRoom; side = -1;
        memset(A, 0, sizeof(A));
        plane = Unready;
    }
    UserInfo(char *_ip, int _port, int _roomid, int _side, char *_A, int _plane)
    {
        memcpy(ip, _ip, IPLength);
        port = _port; roomid = _roomid; side = _side;
        memcpy(A, _A, sizeof(A));
        plane = _plane;
    }
    void operator =(const UserInfo &rhs)
	{
		memcpy(ip, rhs.ip, IPLength);
        port = rhs.port; roomid = rhs.roomid; side = rhs.side;
        memcpy(A, rhs.A, sizeof(rhs.A));
        plane = rhs.plane;
	}
	int operator ==(const UserInfo &rhs) const
	{
		return (!strcmp(ip, rhs.ip) && port == rhs.port);
	}
};

int Mod(int &rhs, const int m);

void reset_daemon();

void init();

int user_login(string username, const char* ip, const int port);
int user_logout(string username, const char* ip, const int port);
