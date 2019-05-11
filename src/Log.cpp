#include "../include/Log.h"

#include <iostream>
#include <cstdio>
#include <cstring>
#include <time.h>
#include <iomanip>

/* public */
Log::Log()
{
    Cur_TimeZone=8;
    strcpy(Logfile_Name,"server.log");
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
            logfile << "===Login Log=== " << "Get a connection from " << ip <<  "[" << port << "], " << Append << endl;
            break;
        case _Disconnect:
            logfile << "===Logout Log=== " << ip << "[" << port << "] disconnected from sever, reason: " << Append << endl;
            break;
        case _Missconnect:
            logfile << "===Logout Log=== " << "Sever miss the connection to " << ip << "[" << port << "]" << endl;
            break;
        case _Reconnect:
            logfile << "===Login Log=== " << "Get a reconnect request from " << ip << "[" << port << "], " << Append << endl;
            break;
        case _Recv:
            logfile << "===Server Log=== " << "Receive " << nbytes << " byte(s) from " << ip << "[" << port << "]" 
                << ", username = " << user << ", status = " << (unsigned int)status << ", op = " << (unsigned int)op << endl;
            break;
        case _Send:
            logfile << "===Server Log=== " << "Send " << nbytes << " bytes(s) to " << ip << "[" << port << "]"
                << ", username = " << user << ", status = " << (unsigned int)status << ", op = " << (unsigned int)op << endl;
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
        case _Invite:
            logfile << "===Game Log=== " << user << "[" << ip << "]" << " send a battle invitation to " << Append << endl;
            break;
        case _Accept:
            logfile << "===Game Log=== "  << user << "[" << ip << "]" << " accept the battle invitation from " << Append << endl;
            break;
        case _Refuse:
            logfile << "===Game Log=== "  << user << "[" << ip << "]" << " refuse the battle invitation from " << Append << endl;
            break;
        case _Join:
            logfile << "===Game Log=== "  << "Room<" << Room_ID << ">: " << user << "[" << ip << "]" << " has joined currently." << endl;
            break;
        case _Leave:
            logfile << "===Game Log=== "  << "Room<" << Room_ID << ">: " << user << "[" << ip << "]" << " has left currently." << endl;
            break;
        case _Ready:
            logfile << "===Game Log=== "  << "Room<" << Room_ID << ">: " << user << "[" << ip << "]" << " has ready." << endl;
            break;
        case _Unready:
            logfile << "===Game Log=== "  << "Room<" << Room_ID << ">: " << user << "[" << ip << "]" << " is unready." << endl;
            break;
        case _GetMap:
            logfile << "===Game Log=== "  << "Room<" << Room_ID << ">: " << "Get " << user << "[" << ip << "]" << " initial map:" << endl;
            logfile << Append;
            break;
        case _Start:
            logfile << "===Game Log=== "  << "Room<" << Room_ID << ">: " << "Game start." << endl;
            break;
        case _Operate:
            logfile << "===Game Log=== "  << "Room<" << Room_ID << ">: " << user << "[" << ip << "]" << " choose " << Append << endl;
            break;
        case _Check:
            logfile << "===Game Log=== "  << "Room<" << Room_ID << ">: " << user << "[" << ip << "]" << " check " << Append << endl;
            break;
        case _Finish:
            logfile << "===Game Log=== "  << "Room<" << Room_ID << ">: " << "Game finished, " << user << "[" << ip << "]" << " win." << endl;
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

    logfile << "[" 
        << 1900 + t->tm_year << "-" 
        << 1 + t->tm_mon << "-" 
        << t->tm_mday << " "
        << setw(2) << setfill('0') << t->tm_hour << ":" 
        << setw(2) << setfill('0') << t->tm_min << ":" 
        << setw(2) << setfill('0') << t->tm_sec << "] ";
}
