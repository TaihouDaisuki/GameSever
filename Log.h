#pragma once

#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstring>
#include <time.h>

#include <string>

using namespace std;

class Log
{
/* member */
public:
    enum _Connect_Type
    {
        _Connect, _Disconnect, _Missconnect, _Reconnect
    };
    enum _Game_Type
    {
        _Join, _Leave, _Ready, _Start, _Pause, _Operate, _Finish
    };

    char ip[20];
    char user[50];
private:
    const int Cur_TimeZone = 8;
    const char Logfile_Name[15] = "server.log";

    ofstream logfile;

/* function */
public:
    Log();
    ~Log();

    int Initialize();
    void Close();

    void Connect(_Connect_Type Type, string Append = "");
    void Game(_Game_Type, const int Room_ID, string Append = "");
private:
    void Get_Time();
};
