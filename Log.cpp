#include "Log.h"

#include <iostream>
#include <cstdio>
#include <cstring>
#include <time.h>

/* public */
Log::Log()
{
    logfile.open(Logfile_Name, ios::app | ios::out);
    if(!logfile)
    {
        cerr << "[ Error, logfile open failed ]" << endl;
        exit(-1);
    }
}
Log::~Log()
{
    logfile << "<----------Logfile close here---------->" << endl << endl << endl;
    logfile.flush();
    logfile.close();
}

void Log::Initialize(const int state)
{
    logfile << "<----------Logfile start here---------->" << endl;
    Get_Time();
    if(!state)
        logfile << "Server starts error" << endl;
    else
        logfile << "Server starts running" << endl;
    logfile.flush();
}
void Log::Close()
{
    Get_Time();
    logfile << "Server will be closed" << endl;
}

void Log::Server_Log(_Server_Type Type, string Append)
{
    Get_Time();
    switch(Type)
    {
        case _Connect:
            logfile << "Get a connection from " << ip <<  "[" << port << "], " << Append << endl;
            break;
        case _Disconnect:
            logfile << ip << "[" << port << "] disconnected from sever, reason: " << Append << endl;
            break;
        case _Missconnect:
            logfile << "Sever miss the connection to " << ip << "[" << port << "]" << endl;
            break;
        case _Reconnect:
            logfile << "Get a reconnect request from " << ip << "[" << port << "], " << Append << endl;
            break;
        case _Recv:
            logfile << "Receive " << nbytes << " byte(s) from " << ip << "[" << port << "]" << endl;
            break;
        case _Send:
            logfile << "Send " << nbytes << " bytes(s) from " << ip << "[" << port << "]" << endl;
            break;
        default:
            break;
    }
    logfile.flush();
}
void Log::Game_Log(_Game_Type Type, const int Room_ID, string Append)
{
    Get_Time();
    switch(Type)
    {
        case _Join:
            logfile << "Room<" << Room_ID << ">: " << user << "[" << ip << "]" << " has joined currently." << endl;
            break;
        case _Leave:
            logfile << "Room<" << Room_ID << ">: " << user << "[" << ip << "]" << " has left currently." << endl;
            break;
        case _Ready:
            logfile << "Room<" << Room_ID << ">: " << user << "[" << ip << "]" << " has ready." << endl;
            break;
        case _Unready:
            logfile << "Room<" << Room_ID << ">: " << user << "[" << ip << "]" << " is unready." << endl;
            break;
        case _GetMap:
            logfile << "Room<" << Room_ID << ">: " << "Get " << user << "[" << ip << "]" << " initial map:" << endl;
            logfile << Append;
            break;
        case _Start:
            logfile << "Room<" << Room_ID << ">: " << "Game start." << endl;
            break;
        case _Pause:
            logfile << "Room<" << Room_ID << ">: " << "Game paused, " << user << "[" << ip << "]" << " miss connection with server" << endl;
            break;
        case _Continue:
            logfile << "Room<" << Room_ID << ">: " << "Game continued, " << user << "[" << ip << "]" << " has reconnected." << endl;
            break;
        case _Operate:
            logfile << "Room<" << Room_ID << ">: " << user << "[" << ip << "]" << " choose " << Append << endl;
            break;
        case _Check:
            logfile << "Room<" << Room_ID << ">: " << user << "[" << ip << "]" << " check " << Append << endl;
            break;
        case _Finish:
            logfile << "Room<" << Room_ID << ">: " << "Game finished, " << user << "[" << ip << "]" << " win." << endl;
            break;
        default:
            break;
    }
    logfile.flush();
}

/* private */
void Log::Get_Time()
{
    time_t tt;
    time(&tt);
    tt += 3600 * Cur_TimeZone;
    tm* t = gmtime(&tt);

    logfile << "[" << 1900 + t->tm_year << "-" << 1 + t->tm_mon << "-" << t->tm_mday << " "
        << t->tm_hour << ":" << t->tm_min << ":" << t->tm_sec << "] ";
}
