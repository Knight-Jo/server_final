#include "databases.h"
#include "GroupManage.h"
#pragma comment(lib,"sqlite3.lib")
/*#include <atlstr.h>
//UTF-8到GB2312的转换
char* U2G(const char* utf8)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len + 1];
	::memset(wstr, 0, len + 1);
	MultiByteToWideChar(CP_UTF8, 0, utf8, -1, wstr, len);
	len = WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len + 1];
	::memset(str, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, wstr, -1, str, len, NULL, NULL);
	if (wstr) delete[] wstr;
	return str;
}

//GB2312到UTF-8的转换
char* G2U(const char* gb2312)
{
	int len = MultiByteToWideChar(CP_ACP, 0, gb2312, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len + 1];
	::memset(wstr, 0, len + 1);
	MultiByteToWideChar(CP_ACP, 0, gb2312, -1, wstr, len);
	len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len + 1];
	::memset(str, 0, len + 1);
	WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
	if (wstr) delete[] wstr;
	return str;
}*/
int main()
{
	// 数据库测试
	/*DataBases d;
	if (!d.ChangeData(1, "Socket=100", "Account=10000002"))
	//各参数含义： 表标识符  修改语句为：属性名=属性值   where语句
	{
		cout << "error" << endl;
	}
	d.SelectData(2);
	//databases中除与图像相关的函数，其他函数的一个参数均为所操作的表，1表示User，2表示Cluster
	if (!d.SelectData(1)) {//不输入where语句代表查询全部的成员    
	                       //输入where语句查询:d.SelectData(1,"Account=10000002")
		cout << "error" << endl;
	}
	else {
		for (int i = 0; i < d.User1.size(); i++) {
			cout << "Account:" << d.User1[i].Account;
			cout << " UName:" << d.User1[i].UName;
			cout << " Password:" << d.User1[i].Password;
			cout << " Potrait:" << d.User1[i].Potrait;
			cout << " LogStatus:" << d.User1[i].LogStatus;
			cout << " Socket:" << d.User1[i].Socket;
			cout << " OffLineTime:" << d.User1[i].OffLineTime;
			cout << endl;
		}
	}
	//Account按从小到大排序，查询到的最后一行的Account就是所有Account中最大的
	//Account可以选择填NULL，它会自动递增，头像的编号应同Acount相同，但不会自动递增
	//数据类型为TEXT的数据传入但没有值时，不要使用NULL而是使用"/0"
	if (!d.AddData(1, "null,'小李','wsxl444',10000003,0,null,'/0'"))
	//各参数含义： 表标识符    插入的数据
	{
		cout << "error" << endl;
	}
	
	if (!d.DeleteData(1, "Account=10000003")) {
	//各参数含义：  表标识符  where语句
		cout << "error" << endl;
	}*/
	

	//GroupManage 测试
	GroupManage G;
	string answer;	
	string input;
	input = "10000002|our";
	answer = G.CreatGroup(input.c_str());
	cout <<"Create Group:" << answer << endl;
	input = "10000002|our";
	answer = G.CreatGroup(input.c_str());
	cout << "Create Group:" << answer << endl;
	input = "10000001|our";
	answer = G.CreatGroup(input.c_str());
	cout << "Create Group:" << answer << endl;
	/*answer = G.Invite("10000002|40000003|10000001");
	cout <<"邀请他人入群:" << answer << endl;
	answer = G.Search("10000000|40000004");
	cout <<"搜索并添加入群:" << U2G(answer.c_str()) << endl;
	answer = G.ShowMember("10000000|40000003");
	cout << "展示群成员" << U2G(answer.c_str()) << endl;
	answer = G.Dissolve("10000002|40000003");
	cout << "解散群" << answer << endl;*/
	
	system("pause");
}