#include <iostream>

#include <map>
#include <list>
#include <string>

#include <stdbool.h>
#include <mysql.h>

#include "Log.h"
#include "Server.h"
#include "Main.h"

Log logop;

map<string, int> usermap; // first-username second-userid
UserInfo userlist[MaxUserNum];
int useri;
map<int, pair<int, int> > roommap; // first-roomid second<Aplayerid, Bplayerid>
int roomlist[MaxRoomNum];
int roomi;

int main()
{
    reset_daemon();
	init();

	int serverstate; /* server initialize */
	logop.Initialize(serverstate);
	if(serverstate == ERROR)
	{
		logop.Close();
		exit(-1);
	}		

	while(1)
	{
		/* recv */
		memcpy(logop.ip, /* ip from packet */, IPLength);
		logop.port = /* port from packet */;
		logop.nbytes = /* nbytes from packet */;
		memcpy(logop.user, /* username from packet */, UserNameLength);
		logop.Server_Log(_Recv);

		if(/* pack = login */)
		{
			string username;
			string password;
			int sqlres;

			sqlres = check_user(username);
			if(!sqlres)
			{
				/* send no such user */
				logop.Server_Log(_Connect, "failed(improper username).");
				continue;
			}
			sqlres = check_password(username, password);
			if(!sqlres)
			{
				/* send password error */
				logop.Server_Log(_Connect, "failed(improper password).");
				continue;
			}
			// login op

			continue;
		}
		if(/* pack = create/join room */)
		{
			
		}
	}

	logop.Close();
    return 0;
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
	roommap.clear();
	useri = 0;
	memset(roomlist, 0, sizeof(roomlist));
	roomi = 0;
}
int Mod(int &rhs, const int m)
{
	return rhs >= m ? rhs-=m : rhs;
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

int create_room(string username)
{
	map<string, int>::iterator userit;
	if(!(userit = usermap.find(username)))
		return ERROR;
	UserInfo &user = userlist[userit->second];

	int lasti = roomi;
	for(int i = 0; i < MaxRoomNum; ++i, Mod(++roomi, MaxRoomNum))
		if(roomlist[i] != Available)
			continue;
	if(roomi == lasti)
		return CreateRoomError;	

	roomlist[roomi] = Waiting;
	roommap.insert(makepair(roomi, makepair(userit->second, Empty)));
	user.roomid = roomi;
	user.side = 0;
	user.plane = Unready;
	return roomi;
}
int join_room(string username, const int roomid)
{
	if(roomid >= MaxRoomNum)
		return ERROR;

	map<string, int>::iterator userit;
	if(!(userit = usermap.find(username)))
		return ERROR;
	UserInfo &user = userlist[userit->second];

	map<int pair<int, int> >::iterator roomit;
	if(!(roomit = roommap.find(roomid)))
		return NoExist;

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
int left_room(string username)
{
	if(roomid >= MaxRoomNum)
		return ERROR;

	map<string, int>::iterator userit;
	if(!(userit = usermap.find(username)))
		return ERROR;
	UserInfo &user = userlist[userit->second];
	
	map<int pair<int, int> >::iterator roomit;
	if(!(roomit = roommap.find(user->roomid)))
		return ERROR;

	user.roomid = NoRoom;
	if(!user.side)
		roomit->second->first = Empty;
	else
		roomit->second->second = Empty;
	
	if((roomit->second->first == Empty) & (roomit->second->second == Empty))
	{
		roomlist[roomit->first] = Available;
		roommap.erase(roomit);
	}
	return OK;
}

int ready_operator(string username, const int isReady)
{
	map<string, int>::iterator userit;
	if(!(userit = usermap.find(username)))
		return ERROR;
	UserInfo &user = userlist[userit->second];

	user.plane = isReady ? Ready : Unready;
	
	map<int pair<int, int> >::iterator roomit;
	if(!(roomit = roommap.find(user.roomid)))
		return ERROR;

	int opponent = user.side ? roomit->second->first : roomit->second->second;
	
	return (user.plane == Ready && opponent != Empty && userlist[opponent].plane == Ready) ? GetMap : Waiting;
}
int start_operator(string username, const char *A, const char *p)
{
	map<string, int>::iterator userit;
	if(!(userit = usermap.find(username)))
		return ERROR;
	UserInfo &user = userlist[userit->second];

	if(!(user->plane == Ready))	
		return ERROR;

	user->plane = PlaneNum;
	memcpy(user.A, A, ChessSize * ChessSize);
	for(int i = 0; i < PlaneNum; ++i)
		for(int j = 0; j <= 1; ++j)
			user.planeX[j][i] = p++, user.plnaeY[j][i] = p++;

	int opponent = user.side ? roomit->second->first : roomit->second->second;
	
	return (userlist[opponent].plane == Ready) ? GetMap : Start;
}
int click_operator(string username, const char X, const char Y)
{
	map<string, int>::iterator userit;
	if(!(userit = usermap.find(username)))
		return ERROR;
	UserInfo &user = userlist[userit->second];
	
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
	UserInfo &user = userlist[userit->second];

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
	if(!(--opponent.plane))
	{
		user->plane = Unready;
		opponent->plane = Unready;
		return GameEnd;
	}
	else
		return Right;
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

int check_user(string username)
{
	string sqlqry;
	sqlqry = "select count(*) from user where account = \'";
	sqlqry += username;
	sqlqry.append("\';");

	mysql_query(mysql, sqlqry.c_str());
    result = mysql_store_result(mysql);
    row = mysql_fetch_row(result);
    int res = atoi(row[0]) ? 1 : 0;

    mysql_free_result(result);

    return res;
}
int check_password(string username, string password)
{
	string sqlqry;
	sqlqry = "select count(*) from user where account = \'";
	sqlqry += username;
	sqlqry.append("\'").append(" and password_MD5 = md5(\'");
	sqlqry += password;
	sqlqry.append("\');");

	mysql_query(mysql, sqlqry.c_str());
    result = mysql_store_result(mysql);
    row = mysql_fetch_row(result);
    int res = atoi(row[0]) ? 1 : 0;

    mysql_free_result(result);

    return res;
}
