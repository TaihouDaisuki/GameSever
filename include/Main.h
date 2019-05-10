#pragma once

#include "Common.h"
#include<sys/types.h> 
#include<sys/stat.h>
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

    int kick;

    int roomid;
    int side;

    int plane;
    char A[ChessSize * ChessSize];
    char planeX[2][PlaneNum], planeY[2][PlaneNum]; // 0-head 1-tail
    char planeflag[PlaneNum];

    char tbuff[tBufferLength];
    
    UserInfo()
    {
        port = NoConnect;
        kick = 0;
        roomid = NoRoom;
        plane = Unready;
        tbuff[0] = NonePack;
    }
};

int Mod(int &rhs, const int m);
string Transform(const int X, const int Y);

void reset_daemon();
void init();

void check_client(time_t &last_check_time);

int work(Server *server, int nbytes, struct sockaddr_in client_addr, char *buff);

int user_login(string username, const char* ip, const int port);
int user_relogin(string username, const char* ip, const int port);
int user_logout(string username, const char* ip, const int port, const int kick = 0);

int get_user_list(const int start_num, const int request_num, string * reslist, int &totnum);
int create_room(string username);
int join_room(string username, const int roomid);
int left_room(UserInfo &user);

int ready_operator(UserInfo &user, const int isReady);
int start_operator(UserInfo &user, const char *p);
int click_operator(UserInfo &user, const int X, const int Y);
int check_operator(UserInfo &user, const char X0, const char Y0, const char X1, const char Y1);

void draw_plane(char *A, const char X0, const char Y0, const char X1, const char Y1);
void fill_plane(char *A, const char X0, const char Y0, const char X1, const char Y1);

int check_user(string username);
int check_password(string username, string password);
