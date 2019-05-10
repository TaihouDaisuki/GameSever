#pragma once

#include "Common.h"

#include <stdbool.h>
#include <mysql.h>
#include <string>

using namespace std;

class Mysql
{
/* member */
public:

private:
    MYSQL     *mysql;   
    MYSQL_RES *result; 
    MYSQL_ROW  row;
/* function */
public:
    Mysql();
    ~Mysql();

    int check_user(string username);
    int check_password(string username, string password);
private:

};
