#pragma once

#include "Common.h"

#include <string>
#include <cstring>

const int deltaX[4][PlaneSize] = 
{
    {0, -2, -1, 0, 1, 2, 0, -1, 0, 1},
    {0, -2, -1, 0, 1, 2, 0, -1, 0, 1},
    {0, 1, 1, 1, 1, 1, 2, 3, 3, 3},
    {0, -1, -1, -1, -1, -1, -2, -3, -3, -3}
};
const int deltaY[4][PlaneSize] = 
{
    {0, 1, 1, 1, 1, 1, 2, 3, 3, 3},
    {0, -1, -1, -1, -1, -1, -2, -3, -3, -3},
    {0, -2, -1, 0, 1, 2, 0, -1, 0, 1},
    {0, -2, -1, 0, 1, 2, 0, -1, 0, 1}
};

struct UserInfo
{
    char ip[IPLength];
    int port;
    int roomid;
    int side;
    char A[ChessSize * ChessSize];
    int plane;
    char planeX[2][PlaneNum], planeY[2][PlaneNum]; // 0-head 1-tail
    
    UserInfo()
    {
        port = NoConnect;
        roomid = NoRoom;
        plane = Unready;
    }
    void operator =(const UserInfo &rhs)
	{
		memcpy(ip, rhs.ip, IPLength);
        port = rhs.port; roomid = rhs.roomid; side = rhs.side;
        memcpy(A, rhs.A, sizeof(rhs.A));
        plane = rhs.plane;
        for(int i = 0; i< PlaneNum; ++i)
            for(int j = 0; j <= 1; ++j)
                planeX[j][i] = rhs.planeX[j][i], planeY[j][i] = rhs.planeY[j][i];
	}
	int operator ==(const UserInfo &rhs) const
	{
		return (!strcmp(ip, rhs.ip) && port == rhs.port);
	}
};

void reset_daemon();
void init();
int Mod(int &rhs, const int m);

int user_login(string username, const char* ip, const int port);
int user_logout(string username, const char* ip, const int port);

int create_room(string username);
int join_room(string username, const int roomid);
int left_room(string username);

int ready_operator(string username, const int isReady);
int start_operator(string username, const char *A, const char *p)
int click_operator(string username, const int X, const int Y);
int check_operator(string username, const char X0, const char Y0, const char X1, const char Y1);
void fill_plane(char *A, const char X0, const char Y0, const char X1, const char Y1);

int check_user(string username);
int check_password(string username, string password);
