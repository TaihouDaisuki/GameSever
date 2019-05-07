#pragma once

#include "Common.h"

#include <fstream>
#include <string>

using namespace std;

class Log
{
/* member */
public:
    enum _Server_Type
    {
        _Connect, _Disconnect, _Missconnect, _Reconnect,
        _Recv, _Send
    };
    enum _Game_Type
    {
        _Join, _Leave, _Ready, _Unready,
        _Start, _Pause, _Continue, _Operate, _Check, _Finish
    };

    char ip[IPLength];
    int port, nbytes;
    char user[UserNameLength];
private:
    const int Cur_TimeZone = 8;
    const char Logfile_Name[15] = "server.log";

    ofstream logfile;

/* function */
public:
    Log();
    ~Log();

    void Initialize(const int state);
    void Close();

    void Server_Log(_Server_Type Type, string Append = "");
    void Game_Log(_Game_Type, const int Room_ID, string Append = "");
private:
    void Get_Time();
};
