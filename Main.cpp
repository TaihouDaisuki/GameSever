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
char operatorbuffer[MaxUserNum][10];
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

		/* login state */
		if(/* pack = login */)
		{
			string username = logop.username;
			string password; /* from packet */
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
				logop.Server_Log(_Connect, "failed(wrong) password).");
				continue;
			}
			// login op
			int res = user_login(username, logop.ip, logop.port);
			//if(res == ERROR) /* too many user, add in future (maybe) */
			if(res == REPEAT) // kick
			{
				if(user_relogin(username, logop.ip, logop.port) == OK)
				{
					/* send user last state */
					continue;
				}
				map<string, int>::iterator userit;
				userit = usermap.find(username);
				/* send kick to userlist[userit->second].ip[userlist[userit->second].port] */
				continue;
			}
			/* common login */
			/* send login success */
			continue;
		}
		if(/* pack = logout */)
		{
			int kickoff; /* from packet */
			user_logout(logop.user, logop.ip, logop.port, kickoff);
		}

		/* searching state */
		if(/* pack = get room list */)
		{
			int start_num; /* from packet */
			int reslist[OnePageRoomNum];
			int resnum = get_room_list(start_num, reslist);
			/* send back room list */
			continue;
		}
		if(/* pack = create room */)
		{
			int roomid = create_room(logop.user);
			// if(roomid == CreateRoomError) /* no more free room, do in future */
			/* send back roomid */
			continue;
		}
		if(/* pack = join room */)
		{
			int roomid; /* get from packet */
			int res = join_room(logop.user, roomid);
			if(res == FULL || res == NoExist)
			{
				/* send back error */
			}
			else
			{
				/* send back roomid */
			}
			
			continue;
		}
		if(/* pack = left room */)
		{
			int res = left_room(logop.user);
			if(res != NoRoom)
				logop.Game_Log(_Leave, res);
			/* send back success */

			continue;
		}

		/* room state */
		if(/* pack = get room state */)
		{
			int ready; /* get from pack */
			int res = ready_operator(logop.user, ready);
			
			map<string, int>::iterator userit;
			userit = usermap.find(logop.user);
			UserInfo &user = userlist[userit->second];
			map<int pair<int, int> >::iterator roomit;
			roomit = roommap.find(user.roomid);
			
			if(res == GetMap)
			{
				/* send to user.ip[user.port] */
				int opponent = user.side ? roomit->second->first : roomit->second->second;
				/* send to userlist[opponent].ip[userlist[opponent].port] */
			}
			
			continue;
		}

		/* start state */
		if(/* pack = chessboard */)
		{
			char A[ChessSize * ChessSize]; /* from packet */
			char p[2 * PlaneNum]; /* from packet */
			int res = start_operator(logop.user, A, p);

			if(res == Start)
			{
				map<string, int>::iterator userit;
				userit = usermap.find(logop.user);
				UserInfo &user = userlist[userit->second];
				map<int pair<int, int> >::iterator roomit;
				roomit = roommap.find(user.roomid);

				/* send to userlist[roomit->second->first] first op (0) */
				/* send to userlist[roomit->second->first] last op (1) */
				logop.Game_Log(_Start, roomit->first);
			}
			continue;
		}

		/* operate state & waiting state */
		if(/* pack = click */)
		{
			char X; /* get from packet */
			char Y; /* get from packet */
			int res = click_operator(logop.user, X, Y);
			
			map<string, int>::iterator userit;
			userit = usermap.find(logop.user);
			UserInfo &user = userlist[userit->second];
			map<int pair<int, int> >::iterator roomit;
			roomit = roommap.find(user.roomid);
			int opponent = user.side ? roomit->second->first : roomit->second->second;

			operatorbuffer[userit->second][0] = 0;
			operatorbuffer[opponent][0] = 1; /* click by other side */
			operatorbuffer[opponent][1] = X;
			operatorbuffer[opponent][2] = Y;

			/* send to operator result by res */

			continue;
		}
		if(/* pack = check */)
		{
			char X0; /* get from packet */
			char Y0; /* get from packet */
			char X1; /* get from packet */
			char Y1; /* get from packet */
			int res = check_operator(logop.user, X0, Y0, X1, Y1);

			map<string, int>::iterator userit;
			userit = usermap.find(logop.user);
			UserInfo &user = userlist[userit->second];
			map<int pair<int, int> >::iterator roomit;
			roomit = roommap.find(user.roomid);
			int opponent = user.side ? roomit->second->first : roomit->second->second;

			operatorbuffer[userit->second][0] = 0;
			if(res != GameEnd)
			{
				operatorbuffer[opponent][0] = 2; /* check by other side */
				operatorbuffer[opponent][1] = X0;
				operatorbuffer[opponent][2] = Y0;
				operatorbuffer[opponent][3] = X1;
				operatorbuffer[opponent][4] = Y1;
			}
			else
			{
				operatorbuffer[opponent][0] = 3; /* Game End */
			}

			/* send to operator result by res */

			continue;
		}
		if(/* pack = empty */)
		{
			map<string, int>::iterator userit;
			userit = usermap.find(logop.user);

			if(operatorbuffer[userit->second][0] == 0)
				/* send back heartbeats */;
			else
				/* send back operatorbuffer */;
			continue;
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
string Transform(const char X, const char Y)
{
	string res = "";
	switch(Y)
	{
		0: res.append("A"); break;
		1: res.append("B"); break;
		2: res.append("C"); break;
		3: res.append("D"); break;
		4: res.append("E"); break;
		5: res.append("F"); break;
		6: res.append("G"); break;
		7: res.append("H"); break;
		8: res.append("I"); break;
		9: res.append("J"); break;
		default:  break;
	}
	switch(X)
	{
		0: res.append("0"); break;
		1: res.append("1"); break;
		2: res.append("2"); break;
		3: res.append("3"); break;
		4: res.append("4"); break;
		5: res.append("5"); break;
		6: res.append("6"); break;
		7: res.append("7"); break;
		8: res.append("8"); break;
		9: res.append("9"); break;
		default:  break;
	}
	return res;
}

int user_login(string username, const char* ip, const int port)
{
	map<string, int>::iterator userit;
	if(userit = usermap.find(username)) // has login
	{
		if(!strcmp(userlist[userit->second].ip, ip) && userlist[userit->second].port == port)
		{
			if(userlist[userit->second].roomid != NoRoom)
				return REPEAT;
			return OK;
		}
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
	userlist[userit->second].kick = 0;
	user.roomid = NoRoom;
	usermap.insert(makepair(username, useri));

	logop.Server_Log(_Connect, username + " login successfully.");
	return OK;
}
int user_relogin(string username, const char* ip, const int port)
{
	map<string, int>::iterator userit;
	if(!(userit = usermap.find(username)))
		return ERROR;
	if(userlist[userit->second].kick == 2)
		return OK;

	if(!userlist[userit->second].kick)
		return ERROR;
	memcpy(userlist[userit->second].ip, ip, IPLength);
	userlist[userit->second].port = port;
	userlist[userit->second].kick = 2;

	logop.Server_Log(_Reconnect, username + " relogin successfully");
	return OK;
}
int user_logout(string username, const char* ip, const int port, const int kick)
{
	map<string, int>::iterator userit;
	if(!(userit = usermap.find(username)))
		return ERROR;
	if(userlist[userit->second].kick == 1)
		return OK;

	if(kick)
	{
		userlist[userit->second].kick = 1;
		logop.Server_Log(_Disconnect, username + " was kicked off.");
	}
	else
	{
		userlist[userit->second].port = NoConnect;
		userlist[userit->second].roomid = NoRoom;
		usermape.erase(userit->first);

		logop.Server_Log(_Disconnect, username + " log out successfully");
	}
	
	return OK;
}

int get_room_list(const int start_num, int * reslist)
{
	map<int pair<int, int> >::iterator roomit;
	roomit = roommap.begin();
	int i = 0, cnt = 0;
	while(roomit != roommap.end())
	{
		if(cnt == start_num + OnePageRoomNum)
			break;
		if((roomit->second->first == Empty) ^ (roomit->second->second == Empty))
			reslist[i++] = roomit->first, ++cnt;
		if(i == OnePageRoomNum)
			i-=OnePageRoomNum;
	}
	return (!i && cnt) ? OnePageRoomNum : i;
}
int create_room(string username)
{
	map<string, int>::iterator userit;
	if(!(userit = usermap.find(username)))
		return ERROR;
	UserInfo &user = userlist[userit->second];
	if(user->roomid != NoRoom)
		return user->roomid;

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

	logop.Game_Log(_Join, roomi);
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
	if(user->roomid == roomid)
		return OK;

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

	logop.Game_Log(_Join, roomid);
	return OK;
}
int left_room(string username)
{
	map<string, int>::iterator userit;
	if(!(userit = usermap.find(username)))
		return ERROR;
	UserInfo &user = userlist[userit->second];
	if(user->roomid == NoRoom)
		return NoRoom;
	
	map<int pair<int, int> >::iterator roomit;
	if(!(roomit = roommap.find(user->roomid)))
		return ERROR;

	int tmproomid = user.roomid;
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
	return tmproomid;
}

int ready_operator(string username, const int isReady)
{
	map<string, int>::iterator userit;
	if(!(userit = usermap.find(username)))
		return ERROR;
	UserInfo &user = userlist[userit->second];

	user.plane = isReady ? Ready : Unready;
	if(isReady)
		logop.Game_Log(_Ready, user.roomid);
	else
		logop.Game_Log(_Unready, user.roomid);
	
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

	if(user->plane != PlaneNum)
	{
		if(!(user->plane == Ready))	
		return ERROR;

		user->plane = PlaneNum;
		memcpy(user.A, A, ChessSize * ChessSize);
		for(int i = 0; i < PlaneNum; ++i)
		{
			for(int j = 0; j <= 1; ++j)
				user.planeX[j][i] = p++, user.plnaeY[j][i] = p++;
			user.planeflag[i] = 1;
		}
		operatorbuffer[userit->second][0] = 0;

		string Map = "";
		for(int i = 0; i < ChessSize; ++i)
			Map.append(A[i * ChessSize], ChessSize).append("\n");
		logop.Game_Log(_GetMap, user->roomid, Map);
	}

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
	if(opponent.A[pos] < 0)
		return -opponent.A[pos];

	string Append = "";
	Append.append("(").append(Transform(X, Y)).append("),  the result is ");
	if(opponent.A[pos] == NoPlane)
		Append.append("Empty.");
	else if(opponent.A[pos] == HitPlane)
		Append.append("Plane.");
	else
		Append.append("Aircraft nose.");
	opponent.A[pos] = -opponent.A[pos];
	logop.Game_Log(_Operate, roomit->first, Append);
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

	int flag = -1;
	for(int i = 0; i < PlaneNum; ++i)
		if(opponent.planeX[0][i] == X0 && opponent.planeY[0][i] == Y0 
			&& opponent.planeX[1][i] == X1 && opponent.planeY[1][i] == Y1)
			{
				flag = i;
				break;
			}
	string Append = "";
	Append.append("(").append(Transform(X0, Y0)).append(")-(").append(Transform(X1, Y1)).append("), the result is ");
	if(flag == -1)
	{
		Append.append("wrong");
		logop.Game_Log(_Check, roomit->first, Append);
		return Wrong;
	}
	if(!opponent.planeflag[flag])
		return Right;
	
	Append.append("right");
	logop.Game_Log(_Check, roomit->first, Append);
	fill_plane(opponent.A, X0, Y0, X1, Y1);
	if(!(--opponent.plane))
	{
		user->plane = Unready;
		opponent->plane = Unready;
		logop.Game_Log(_Finish, roomit->first);
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
