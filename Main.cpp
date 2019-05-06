#include <iostream>

#include <map>
#include <list>
#include <string>

#include "Log.h"
#include "Server.h"
#include "Main.h"

map<string, int> usermap; // first-username second-userid
UserInfo userlist[MaxUserNum];
int useri, usercnt;
map<int, pair<int, int> > roommap; // first-roomid second<Aplayerid, Bplayerid>
int roomi, roomcnt;

int main()
{
    reset_daemon();
	init();


    return 0;
}

int Mod(int &rhs, const int m)
{
	return rhs >= m ? rhs-=m : rhs;
}

void reset_daemon()
{
	pid_t pid;
	if((pid = fork()))
		exit(0);	//是父进程，则结束父进程
	else if(pid < 0)
		exit(1);	//fork失败，退出
	
	//是第一子进程，后台继续执行
	setsid();		//第一子进程成为新的会话组长和进程组长
	//与控制终端分离
	if((pid = fork()))
		exit(0);	//是第一子进程，结束第一子进程
	else if(pid < 0)
		exit(1);	//fork失败，退出
	//是第二子进程，继续。第二子进程不再是会话组长
	
	//for(i = 0; i < NOFILE; ++i)
	//	close(i);
		//关闭从父进程继承打开的文件描述符，节省系统资源
		//但这里需要输出回显，因此不关闭

	//chdir("/root/");	//改变工作目录到root，这里只需要打印，所以不需要
	umask(0);			//重设文件创建掩模，防止守护进程创建的文件存取位被父进程修改
	return;
}

void init()
{
	usermap.clear();
	useri = usercnt = 0;
	roomi = roomcnt = 0;
}

int user_login(string username, const char* ip, const int port)
{
	map<string, int>::iterator it;
	if(it = usermap.find(username)) // has login
	{
		// send logout to (userlist[it->seconde].ip, userlist[it->seconde].port)
		userlist[it->second].port = NoConnect;
		usermap.erase(it->first);
		return REPEAT;
	}
	
	int lasti = useri;
	for(int i = 0; i < MaxUserNum; ++i, Mod(++useri, MaxUserNum))
		if(userlist[i].port != NoConnect)
			continue;
	if(useri == lasti)
		return ERROR;
	userlist[useri] = UserInfo(ip, port, NoRoom, NoRoom, "0", Unready);
	usermap.insert(makepair(username, useri));
	return OK;
}
int user_logout(string username, const char* ip, const int port)
{
	map<string, int>::iterator it;
	if(!(it = usermap.find(username)))
		return ERROR;
	
	return OK;
}
