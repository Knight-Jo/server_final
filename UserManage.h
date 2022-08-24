#ifndef _USER_MANAGE_H
#define _USER_MANAGE_H
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string.h>
#include <vector>
#include <time.h>
#include "sqlite3.h"
#include "databases.h"

using namespace std;


class UserManage {
public:
	string  Register(string buf);
	string  SearchUser(string buf);
	string  AddFriend1(string buf);
	string  AddFriend2(string buf);
	string  ChangeData(string buf);
	string  Logging(string buf);
	string LogSuccess(string buf);
	string LogOut(string buf);
	string  Receive(string buf, vector<int>& Members);
	string  SendMsg(string buf, int confd);//不同后缀的发送消息有不同处理
	string  SelecteAccount(string buf);
	string  SelecteGroup(string buf);
protected:

private:
	DataBases D;
};
#endif // _USER_MANAGE_H
