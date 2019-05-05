#include "Log.h"

/* public */
Log::Log()
{
    // nothing to do
}
Log::~Log()
{
    // nothing to do
}

int Log::Initialize()
{
    logfile.open(Logfile_Name, ios::app | ios::out);
    if(!logfile)
    {
        cerr << "[ Error, logfile open failed ]" << endl;
        return 0;
    }
    logfile << "<----------Logfile start here---------->" << endl;
    Get_Time();
    logfile << "Server starts running" << endl;
    logfile.flush();

    return 1;
}
void Log::Close()
{
    Get_Time();
    logfile << "Server will be closed" << endl;
    logfile << "<----------Logfile close here---------->" << endl << endl << endl;
    logfile.flush();
    logfile.close();
}

void Log::Connect(_Connect_Type Type, string Append)
{
    Get_Time();
    switch(Type)
    {
        case _Connect:
            logfile << "Get a connection from [ip = " << ip <<  "], " << Append << endl;
            break;
        case _Disconnect:
            logfile << "[ip = " << ip << "] disconnected from sever, " << Append << endl;
            break;
        case _Missconnect:
            logfile << "Sever miss the connection to [ip = " << ip << "]" << endl;
            break;
        case _Reconnect:
            logfile << "Get a reconnect request from [ip = " << ip << "], " << Append << endl;
            break;
        default:
            break;
    }
    logfile.flush();
}
void Log::Game(_Game_Type Type, const int Room_ID, string Append)
{
    Get_Time();
    switch(Type)
    {
        case _Join:
            logfile << "Room<" << Room_ID << ">: " << user << "[" << ip << "]" << "has joined currently." << endl;
            break;
        case _Leave:
            logfile << "Room<" << Room_ID << ">: " << user << "[" << ip << "]" << "has left currently." << endl;
            break;
        case _Ready:
            logfile << "Room<" << Room_ID << ">: " << user << "[" << ip << "]" << "has ready currently." << endl;
            logfile << "The initial statement is: " << endl;
            logfile << Append << endl;
            break;
        case _Start:
            logfile << "Room<" << Room_ID << ">: " << "Game start." << endl;
            break;
        case _Pause:
            logfile << "Room<" << Room_ID << ">: " << "Game paused, " << user << "[" << ip << "]" << "miss connection with server" << endl;
            break;
        case _Operate:
            logfile << "Room<" << Room_ID << ">: " << user << "[" << ip << "]" << " choose " << Append << endl;
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
