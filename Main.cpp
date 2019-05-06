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
	map<string, int>::iterator userit;
	if(userit = usermap.find(username)) // has login
	{
		// send logout to (userlist[it->seconde].ip, userlist[it->seconde].port)
		memcpy(userlist[userit->second].ip, ip, IPLength);
		userlist[userit->second].port = port;
		return REPEAT;
	}
	
	int lasti = useri;
	for(int i = 0; i < MaxUserNum; ++i, Mod(++useri, MaxUserNum))
		if(userlist[i].port != NoConnect)
			continue;
	if(useri == lasti)
		return ERROR;
	
	UserInfo &user = userlist[userit->second];
	memcpy(user.ip, ip, IPLength);
	user.port = port;
	user.roomid = NoRoom;
	usermap.insert(makepair(username, useri));
	return OK;
}
int user_logout(string username, const char* ip, const int port)
{
	map<string, int>::iterator userit;
	if(!(userit = usermap.find(username)))
		return ERROR;

	userlist[userit->second].port = NoConnect;
	usermape.erase(userit->first);
	return OK;
}

int join_room(string username, const int roomid, const int create)
{
	if(roomid >= MaxRoomNum)
		return ERROR;

	map<string, int>::iterator userit;
	if(!(userit = usermap.find(username)))
		return ERROR;
	UserInfo &user = userlist[userit->second];

	map<int pair<int, int> >::iterator roomit;
	if(roomit = roommap.find(roomid))
	{
		if(!((roomit->second->first == Empty) | (roomit->second->second == Empty)))
			return Full;
		
		if(roomit->second->first == Empty)
		{
			roomit->second->first = userit->second;
			user.side = 0;
		}
		else
		{
			roomit->second->second = userit->second;
			user.side = 1;
		}
		user.roomid = roomid;
		user.plane = Unready;

		return OK;
	}

	if(!create)
		return NoExist;
	roommap.insert(makepair(roomid, makepair(userit->second, Empty)));
	user.roomid = roomid;
	user.side = 0;
	user.plane = Unready;
	return OK;
}
int left_room(string username, const int roomid)
{
	if(roomid >= MaxRoomNum)
		return ERROR;

	map<string, int>::iterator userit;
	if(!(userit = usermap.find(username)))
		return ERROR;
	serInfo &user = userlist[userit->second];
	
	map<int pair<int, int> >::iterator roomit;
	if(!(roomit = roommap.find(roomid)))
		return ERROR;

	user.roomid = NoRoom;
	if(!user.side)
		roomit->second->first = Empty;
	else
		roomit->second->second = Empty;
	
	if((roomit->second->first == Empty) & (roomit->second->second == Empty))
		roommap.remove(roomit);
	return OK;
}

int ready_operator(string username, const char *A, const char *p)
{
	map<string, int>::iterator userit;
	if(!(userit = usermap.find(username)))
		return ERROR;
	serInfo &user = userlist[userit->second];

	user.plane = PlaneNum;
	memcpy(user.A, A, ChessSize * ChessSize);
	for(int i = 0; i < PlaneNum; ++i)
		for(int j = 0; j <= 1; ++j)
			user.planeX[j][i] = p++, user.plnaeY[j][i] = p++;

	map<int pair<int, int> >::iterator roomit;
	if(!(roomit = roommap.find(user.roomid)))
		return ERROR;

	int opponent = user.side ? roomit->second->first : roomit->second->second;
	
	return (opponent != Empty && userlist[opponent].plane == PlaneNum) ? Start : Waiting;
}
int click_operator(string username, const char X, const char Y)
{
	map<string, int>::iterator userit;
	if(!(userit = usermap.find(username)))
		return ERROR;
	serInfo &user = userlist[userit->second];
	
	map<int pair<int, int> >::iterator roomit;
	if(!(roomit = roommap.find(user.roomid)))
		return ERROR;
	UserInfo &opponent = user.side ? userlist[roomit->second->first] : userlist[roomit->second->second];

	int pos = ChessSize * X + Y;
	opponent.A[pos] = -opponent.A[pos];
	return -opponent.A[pos];
}
int check_operator(string username, const char X0, const char Y0, const char X1, const char Y1)
{
	map<string, int>::iterator userit;
	if(!(userit = usermap.find(username)))
		return ERROR;
	serInfo &user = userlist[userit->second];

	map<int pair<int, int> >::iterator roomit;
	if(!(roomit = roommap.find(user.roomid)))
		return ERROR;
	UserInfo &opponent = user.side ? userlist[roomit->second->first] : userlist[roomit->second->second];

	int res = 0;
	for(int i = 0; i < PlaneNum; ++i)
		if(opponent.planeX[0][i] == X0 && opponent.planeY[0][i] == Y0 
			&& opponent.planeX[1][i] == X1 && opponent.planeY[1][i] == Y1)
			{
				res = 1;
				break;
			}
	if(res == 0)
		return Wrong;
	
	fill_plane(opponent.A, X0, Y0, X1, Y1);
	return (--opponent.plane) ? Right : GameEnd;
}
void fill_plane(char *A, const char X0, const char Y0, const char X1, const char Y1)
{
	int basepos = X0 * ChessSize + Y0;
	int k, pos;
	if(X0 == X1)
		k = Y0 < Y1 ? 0/* up */ : 1/* down */;
	else
		k = X0 < X1 ? 2/* left */ : 3/* right */;
	for(int i = 0; i < PlaneSize; ++i)
	{
		pos = basepos + deltaX[k][i] * ChessSize + deltaY[k][i];
		A[pos] = -A[pos];
	}		
}
