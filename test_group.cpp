#include "databases.h"
#include "GroupManage.h"
#pragma comment(lib,"sqlite3.lib")
/*#include <atlstr.h>
//UTF-8��GB2312��ת��
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

//GB2312��UTF-8��ת��
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
	// ���ݿ����
	/*DataBases d;
	if (!d.ChangeData(1, "Socket=100", "Account=10000002"))
	//���������壺 ���ʶ��  �޸����Ϊ��������=����ֵ   where���
	{
		cout << "error" << endl;
	}
	d.SelectData(2);
	//databases�г���ͼ����صĺ���������������һ��������Ϊ�������ı�1��ʾUser��2��ʾCluster
	if (!d.SelectData(1)) {//������where�������ѯȫ���ĳ�Ա    
	                       //����where����ѯ:d.SelectData(1,"Account=10000002")
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
	//Account����С�������򣬲�ѯ�������һ�е�Account��������Account������
	//Account����ѡ����NULL�������Զ�������ͷ��ı��ӦͬAcount��ͬ���������Զ�����
	//��������ΪTEXT�����ݴ��뵫û��ֵʱ����Ҫʹ��NULL����ʹ��"/0"
	if (!d.AddData(1, "null,'С��','wsxl444',10000003,0,null,'/0'"))
	//���������壺 ���ʶ��    ���������
	{
		cout << "error" << endl;
	}
	
	if (!d.DeleteData(1, "Account=10000003")) {
	//���������壺  ���ʶ��  where���
		cout << "error" << endl;
	}*/
	

	//GroupManage ����
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
	cout <<"����������Ⱥ:" << answer << endl;
	answer = G.Search("10000000|40000004");
	cout <<"�����������Ⱥ:" << U2G(answer.c_str()) << endl;
	answer = G.ShowMember("10000000|40000003");
	cout << "չʾȺ��Ա" << U2G(answer.c_str()) << endl;
	answer = G.Dissolve("10000002|40000003");
	cout << "��ɢȺ" << answer << endl;*/
	
	system("pause");
}