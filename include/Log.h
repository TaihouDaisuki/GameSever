//#pragma once

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
        _Invite, _Accept, _Refuse,
        _Join, _Leave, _Ready, _Unready,
        _GetMap, _Start,
         _Operate, _Check, _Finish
    };

    char ip[IPLength];
    int port, nbytes;
    string user;
private:
    int Cur_TimeZone;
    char Logfile_Name[15];

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
