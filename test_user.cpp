#include <iostream>
#include <string>
#include <vector>
#include "sqlite3.h"
#include "databases.h"
#include "UserManage.h"

int main()
{
	UserManage U;
	vector<int> Members;

	string buf5 = "小A|password";
	const char *reg_buf1 = "gdw|123" ; 
	const char *reg_buf2 = "wyn|123" ; 
	string reg_buf3 = "yrr|123" ; 
	string buf9 = "10000002|wsxq789|88888";	
	string buf101 = "10000002|10000001|2022.10.10.12.12.11|再见.";
	string buf102 = "10000001|10000002|2022.10.10.11.11.22|再见.";
	string buf6 = "10000000|10000005";	
	string buf71 = "10000001|10000008";	
	string buf72 = "reject|10000000|10000002"; 
	string buf8 = "10000000|b|Mike123";  
	string buf12 = "10000000"; 
	string buf13 = "10000000|10000002"; 

	/* 注册 */
	string reg_ans1 = U.Register(reg_buf1) ; 
	string reg_ans2 = U.Register(reg_buf2) ; 
	string reg_ans3 = U.Register(reg_buf3) ; 

	cout <<"register : " <<endl ; 
	cout<<"\t" << reg_ans1<<endl ; 
	cout <<"\t" << reg_ans2 <<endl ; 
	cout <<"\t" << reg_ans3 <<endl ;

	string ans9 = U.Logging(buf9);
	string ans101 = U.Receive(buf101, Members);
	string ans102 = U.Receive(buf102, Members);
	string ans6 = U.SearchUser(buf6);
	string ans71 = U.AddFriend1(buf71);
	string ans8 = U.ChangeData(buf8);
	string ans12 = U.SelecteAccount(buf12);
	string ans13 = U.SelecteGroup(buf13);

	cout <<"ans9 "<<endl ; 
	cout << ans9 << endl;
	cout << ans101 << endl;
	cout << ans102 << endl;
	cout << ans6 << endl;
	cout<<"ans71"<<endl ; 
	cout << ans71 << endl;
	cout << ans8 << endl;
	cout << ans12 << endl;
	cout << ans13 << endl;

	return 0;
}
