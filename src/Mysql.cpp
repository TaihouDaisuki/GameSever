#include "Common.h"
#include "Mysql.h"

#include <iostream>

using namespace std;

Mysql::Mysql()
{
    if ((mysql = mysql_init(NULL)) == NULL)
    {
        cerr << "mysql_init failed" << endl;
        exit(EXIT_FAILURE);
    }

    if (mysql_real_connect(mysql, "localhost", "u1650254", "u1650254", "db1650254", 0, NULL, 0) == NULL)
    {
        cerr << "mysql_real_connect failed(" << mysql_error(mysql) << ")" << endl;
        exit(EXIT_FAILURE);
    }
    mysql_set_character_set(mysql, "gbk");
}
Mysql::~Mysql()
{
    mysql_close(mysql);
}

int Mysql::check_user(string username)
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
int Mysql::check_password(string username, string password)
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
