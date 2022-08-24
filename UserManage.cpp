#include "UserManage.h"

//return: ���#������������
string UserManage::Register(string buf) //buf:nickname|password; return:5#�˺�(string)
{
	char* Nickname = new char[buf.length() + 1];
	char* Password = new char[buf.length() + 1];
	char* Buffer = new char[buf.length() + 1];
	strcpy(Buffer, buf.c_str());
	sscanf(Buffer, "%[^|]|%s", Nickname, Password);
	string NickName = Nickname;
	string PassWord = Password;

	D.SelectData(1, "/0");	
	size_t n = D.User1.size() - 1;	
	int MaxAccount = D.User1[n].Account;

	string data = to_string(MaxAccount + 1) + ",'" + NickName + "','" + PassWord + "'," +"0" + ",0," + "0" + "," + "'/0'";
	D.AddData(1, data);	

	delete[] Nickname;
	delete[] Password;
	delete[] Buffer;
	return string("5#" + to_string(MaxAccount + 1));
}

string UserManage::Logging(string buf)//buf:�˺�|����|Socket; return:9#LogSuccess��return / 9#Failed
{
	char* acc = new char[buf.length() + 1];
	char* pword = new char[buf.length() + 1];
	char* socket = new char[buf.length() + 1];
	char* Buffer = new char[buf.length() + 1];
	strcpy(Buffer, buf.c_str());
	sscanf(Buffer, "%[^|]|%[^|]|%s", acc, pword, socket);
	string Acc = acc;
	string PWord = pword;
	string SK = socket;

	string SQL = "Account = " + Acc;
	D.SelectData(1, SQL);

	string ans;
	if (!D.User1.empty())
	{
		string Password = D.User1[0].Password;
		if (Password == PWord)
		{
			ans = "9#";
			ans += LogSuccess(buf);
		}
		else
		{
			ans = "9#Failed";
		}
	}
	else
	{
		ans = "9#Failed";
	}
	delete[] acc;
	delete[] pword;
	delete[] Buffer;
	delete[] socket;
	return ans;
}


string UserManage::LogSuccess(string buf)//buf:�˺�|����|Socket; return:�˺�#�ǳ�#�����˺�1|�����˺�2|#�����ǳ�1|�����ǳ�2|#Ⱥ��1|Ⱥ��2|#Ⱥ��1|Ⱥ��2|#δ������Ϣ1|δ������Ϣ2|��δ������Ϣ��ʱ��,������Ϣ���û����˺�,������Ϣ���û����ǳ�,Ⱥ�Ż�����˺�,�Ի���Ϣ
{
	char* acc = new char[buf.length() + 1];
	char* pword = new char[buf.length() + 1];
	char* socket = new char[buf.length() + 1];
	char* Buffer = new char[buf.length() + 1];
	strcpy(Buffer, buf.c_str());
	sscanf(Buffer, "%[^|]|%[^|]|%s", acc, pword, socket);
	string Acc = acc;
	string PWord = pword;
	string SK = socket;
	delete[] acc;
	delete[] pword;
	delete[] Buffer;
	delete[] socket;
	//������Ϣ
	int Account ;
	string UName, OffLineTime;
	D.SelectData(1, "Account = " + Acc);
	Account = D.User1[0].Account;
	UName = D.User1[0].UName;
	OffLineTime = D.User1[0].OffLineTime;

	string Data1 = to_string(Account) + "#" + UName;

	//�ı��¼״̬����¼socket
	D.ChangeData(1, "LogStatus=1", "Account=" + Acc);
	D.ChangeData(1, "Socket=" + SK, "Account=" + Acc);

	//δ������Ϣ
	string UnSendMsg = "";
	string SQL = "CMember LIKE '%" + Acc + "%'";
	D.SelectData(2, SQL);
	for (size_t i = 0; i < D.Cluster1.size(); i++)
	{
		if (i >= D.Cluster1.size() || i < 0) break;
		string RNum;
		string MsgRecord = D.Cluster1[i].MsgRecord;
		int COwner = D.Cluster1[i].COwner;
		if (COwner == 1)//���ѹ�ϵ
		{
			string CMember = D.Cluster1[i].CMember;
			char* N = new char[CMember.length() + 1];
			char* F = new char[CMember.length() + 1];
			char* B = new char[CMember.length() + 1];
			strcpy(B, CMember.c_str());
			sscanf(B, "%[^|]|%s", N, F);
			string Fr = F, NA = N;
			if (NA == Acc)//��ֳ�����N�ǵ�ǰ�û��˺ţ���F�Ǻ����˺�
			{
				RNum = Fr;
			}
			else
			{
				RNum = NA;
			}
			delete[] N;
			delete[] F;
			delete[] B;
		}
		else//Ⱥ�Ĺ�ϵ
		{
			RNum = to_string(D.Cluster1[i].CNum);
		}
		
		char* MR = new char[MsgRecord.length() + 1];
		strcpy(MR, MsgRecord.c_str());
		char* AMsg;
		AMsg = strtok(MR, "|");//�ַ������strtok()
		while (AMsg != NULL)	//AMsg:Time1,Acount1,Msg1
		{
			string AM = AMsg;
			char* time = new char[AM.length() + 1];
			char* acc = new char[AM.length() + 1];
			char* msg = new char[AM.length() + 1];
			sscanf(AMsg, "%[^,],%[^,],%s", time, acc, msg);
			string Time = time;
			string Acc = acc;
			string Msg = msg;
			delete[] time;
			delete[] acc;
			delete[] msg;

			D.SelectData(1, "Account = " + Acc);
			string NickName = D.User1[0].UName;
			string SMsg2 = Time + "," + Acc + "," + NickName + "," + RNum + "," + Msg;
			if (Time > OffLineTime)	//�����Ϣʱ�������ϴ�����ʱ��   ���ַ����Ƚ�����λ�Ƚϣ�
			{
				UnSendMsg += SMsg2 + "|";
			}
			AMsg = strtok(NULL, "|");
		}
		delete[] MR;
	}
	//����
	string SQL1 = "CMember LIKE '%" + Acc + "%' AND COwner=1";//������Acc������Ⱥ��
	D.SelectData(2, SQL1);
	string AllFriend = "";
	string AllFriendNick = "";
	for (size_t i = 0; i < D.Cluster1.size(); i++)
	{
		if (i >= D.Cluster1.size() || i < 0) break;
		char* a = new char[D.Cluster1[i].CMember.length() + 1];
		char* f = new char[D.Cluster1[i].CMember.length() + 1];
		char* Buffer = new char[D.Cluster1[i].CMember.length() + 1];
		strcpy(Buffer, D.Cluster1[i].CMember.c_str());
		sscanf(Buffer, "%[^|]|%s", a, f);
		string A = a;
		string F = f;
		delete[] a;
		delete[] f;
		delete[] Buffer;
		if (A == Acc)
		{
			AllFriend += F + "|";
			D.SelectData(1, "Account=" + F);
			AllFriendNick += D.User1[0].UName + "|";
		}
		else
		{
			AllFriend += A + "|";
			D.SelectData(1, "Account=" + A);
			AllFriendNick += D.User1[0].UName + "|";
		}
	}
	//Ⱥ��
	string SQL2 = "CMember LIKE '%" + Acc + "%' AND COwner<>1";
	D.SelectData(2, SQL2);
	string AllGroup = "";
	string AllGroupName = "";
	for (size_t i = 0; i < D.Cluster1.size(); i++)
	{
		if (i >= D.Cluster1.size() || i < 0) break;
		AllGroup += to_string(D.Cluster1[i].CNum) + "|";
		AllGroupName += D.Cluster1[i].CName + "|";
	}

	return Data1 + "#" + AllFriend + "#" + AllFriendNick + "#" + AllGroup + "#" + AllGroupName + "#" + UnSendMsg;
}

string UserManage::LogOut(string buf)//buf:�˺�; return:(�ޣ�
{
	string Acc = buf;
	//�ı��¼״̬
	D.ChangeData(1, "LogStatus=0", "Account=" + Acc);
	//��¼����ʱ��
	time_t timep;
	time(&timep);
	char tmp[64];
	strftime(tmp, sizeof(tmp), "%Y.%m.%d.%H.%M.%S", localtime(&timep));
	string time = tmp;//��ȡ��ǰʱ��
	D.ChangeData(1, "OffLineTime='" + time + "'", "Account=" + Acc);
	return string();
}


string UserManage::Receive(string buf, vector<int>& Members)//buf:Ⱥ�Ż�����˺�|�û��˺�|����ʱ��|��Ϣ����; return:10#������Ϣ���û����˺�#Ⱥ�Ż�����˺�#ʱ��#�Ի���Ϣ
{
	char* num = new char[buf.length() + 1];
	char* acc = new char[buf.length() + 1];
	char* time = new char[buf.length() + 1];
	char* msg = new char[buf.length() + 1];
	char* Buffer = new char[buf.length() + 1];
	strcpy(Buffer, buf.c_str());
	sscanf(Buffer, "%[^|]|%[^|]|%[^|]|%s", num, acc, time, msg);
	string Num = acc, Acc = num, Time = time, Msg = msg;
	//ȷ��Ⱥ����
	string CNum;
	if (Num < "40000000")//Num�Ǻ����˺�
	{
		string Member1 = Acc + "|" + Num;
		string Member2 = Num + "|" + Acc;
		string SQL = "CMember = '" + Member1 + "'or CMember = '" + Member2 + "'";
		D.SelectData(2, SQL);

		int Num = D.Cluster1[0].CNum;
		CNum = to_string(Num);
	}
	else
	{
		CNum = Num;
	}

	string SMsg = Time + "," + Acc + "," + Msg;//�������ݿ�ĸ�ʽ
	D.SelectData(1, "Account=" + Acc) ; 
	string NickName = D.User1[0].UName ; 
	string SMsg2 =  NickName+ "#" +Time +"#" + Msg ;//���ظ��ͻ��˵ĸ�ʽ
	//������������¼
	D.SelectData(2, "CNum = " + CNum);
	string MsgRecord = D.Cluster1[0].MsgRecord;//Time1,Acount1,Msg1|Time2,Account2,Msg2|Time3,Account3,Msg3
	string RMsg;
	if (MsgRecord == "/0")
	{
		RMsg = "'" + SMsg + "'";
	}
	else
	{
		RMsg = "'" + MsgRecord + "|" + SMsg + "'";
	}
	D.ChangeData(2, "MsgRecord = " + RMsg, "CNum = " + CNum);

	D.SelectData(2, "CNum = " + CNum);
	string CMember = D.Cluster1[0].CMember;
	char* CM = new char[CMember.length() + 1];
	strcpy(CM, CMember.c_str());
	char* User;
	User = strtok(CM, "|");//�ַ������strtok()
	while (User != NULL)
	{
		string U = User ; 
		if(U!= Acc){
			Members.push_back(atoi(User));
		}
		User = strtok(NULL, "|");
	}

	delete[] num;
	delete[] acc;
	delete[] time;
	delete[] msg;
	delete[] Buffer;
	delete[] CM;
	return SMsg2;
}

string UserManage::SearchUser(string buf)//buf:���û��˺�|���������˺�; return : 6#�˺�#�ǳ�#��¼״̬#�ϴ�����ʱ��/ ���û�������
{
	char* now = new char[buf.length() + 1];
	char* search = new char[buf.length() + 1];
	char* Buffer = new char[buf.length() + 1];
	strcpy(Buffer, buf.c_str());
	sscanf(Buffer, "%[^|]|%s", now, search);
	string Now = now, Search = search;
	delete[] now;
	delete[] search;
	delete[] Buffer;
	D.SelectData(1, "Account = " + Now);

	string data = "";
	string tmpdata;
	int Account,  LogStatus;
	string UName, OffLineTime;
	D.SelectData(1, "Account = " + Search)  ; 
	if ( !D.User1.empty())
	{
		Account = D.User1[0].Account;
		LogStatus = D.User1[0].LogStatus;
		UName = D.User1[0].UName;
		OffLineTime = D.User1[0].OffLineTime;
		data = "6#" + to_string(Account) + "#" + UName + "#" + to_string(LogStatus) + "#" + OffLineTime;
		return data;
	}
	else
	{
			return "6#The User Does Not Exist!";
	}
}

string UserManage::AddFriend1(string buf)//buf:�û��˺�|�����˺ţ�return : 7#Y#�����˺�#�����ǳ�/N(�Ѿ��Ǻ��ѣ�
{
	char* acc1 = new char[buf.length() + 1];
	char* acc2 = new char[buf.length() + 1];
	char* Buffer = new char[buf.length() + 1];
	strcpy(Buffer, buf.c_str());
	sscanf(Buffer, "%[^|]|%s", acc1, acc2);
	string Acc1 = acc1, Acc2 = acc2;
	delete[] acc1;
	delete[] acc2;
	delete[] Buffer;

	//�ж������Ƿ��Ѿ��Ǻ���
	string Member1 = Acc1 + "|" + Acc2;
	string Member2 = Acc2 + "|" + Acc1;
	string SQL = "CMember = '" + Member1 + "'or CMember = '" + Member2 + "'";
	D.SelectData(2, SQL);
	if (!D.Cluster1.empty())
	{
		return "7#N";
	}

	string data;
	string Member = Acc1 + "|" + Acc2;

	D.SelectData(2, "/0");
	size_t n = D.Cluster1.size() - 1;
	int MaxCNum = D.Cluster1[n].CNum;

	data = to_string(MaxCNum + 1) + ",'/0', " + to_string(1) + ", '" + Member + "', " + "'/0'";
	D.AddData(2, data);

	D.SelectData(1, "Account=" + Acc2);
	string UNum = to_string(D.User1[0].Account);
	string UName = D.User1[0].UName;
	
	return "7#Y#" + UNum + "#" + UName;
}
/*
string UserManage::AddFriend2(string buf)//buf:accept/reject|�û��˺�|�����˺ţ�return : 7#accept/reject
{
	char* acc1 = new char[buf.length() + 1];
	char* acc2 = new char[buf.length() + 1];
	char* ar = new char[buf.length() + 1];
	char* Buffer = new char[buf.length() + 1];
	strcpy(Buffer, buf.c_str());
	sscanf(Buffer, "%[^|]|%[^|]|%s", ar, acc1, acc2);
	string Acc1 = acc1, Acc2 = acc2, A_R = ar;

	D.SelectData(1, "Account = " + Acc1);
	int Socket = D.User1[0].Socket;
	D.SelectData(1, "Account = " + Acc2);
	string UName = D.User1[0].UName;

	if (A_R == "reject")
	{
		bool b = D.DeleteData(2, "CMember = '" + Acc1 + "|" + Acc2 + "'");

		string SMsg = "7#" + UName + "," + Acc2 + ",rejected your invitation.";
		SendMsg(SMsg, Socket);//(������ѣ����������ˣ�7#���������ǳ�,���������˺�,rejected/accepted your invitation.
		return "7#reject";
	}
	else if (A_R == "accept")
	{
		string SMsg = "7#" + UName + "," + Acc2 + ",accepted your invitation.";
		SendMsg(SMsg, Socket);//(������ѣ����������ˣ�7#���������ǳ�,���������˺�,rejected/accepted your invitation.
		return "7#accept";
	}
}
*/
string UserManage::ChangeData(string buf)//buf:�û��˺�|Ҫ�޸ĵ�������|�޸ĺ������;return : 8#Success(�ɹ���ɣ�
{
	char* acc = new char[buf.length() + 1];
	char* col = new char[buf.length() + 1];
	char* cdata = new char[buf.length() + 1];
	char* Buffer = new char[buf.length() + 1];
	strcpy(Buffer, buf.c_str());
	sscanf(Buffer, "%[^|]|%[^|]|%s", acc, col, cdata);
	string Acc = acc, Col = col, CData = cdata;
	delete[] acc;
	delete[] col;
	delete[] cdata;
	delete[] Buffer;

	if (Col == "a")
	{
		D.ChangeData(1, "UName = '" + CData + "'", "Account = " + Acc);
	}
	else
	{
		D.ChangeData(1, "Password = '" + CData + "'", "Account = " + Acc);
	}
	

	return "8#Success";
}

string UserManage::SelecteAccount(string buf)//buf:�û��˺�(string);return : 12#Success(�ɹ���ɣ�
{
	D.DeleteData(1, "Account = " + buf);//ɾ�����û�
	//ɾ�����û������к��ѹ�ϵ
	string SQL = "CMember LIKE '%" + buf + "%' AND COwner = 1";
	D.DeleteData(2, SQL);
	//��������Ⱥ���Ⱥ��Ա:		Member1|Member2|Member3|
	string SQL1 = "CMember LIKE '%" + buf + "%' AND COwner != 1";
	D.SelectData(2, SQL1);
	size_t n = D.Cluster1.size();
	for (size_t i = 0; i < n; i++)
	{
		string CNum = to_string(D.Cluster1[i].CNum);
		string COwner = to_string(D.Cluster1[i].COwner);
		if (COwner == buf)
		{
			D.DeleteData(2, "CNum=" + CNum);
		}
		string CMember = D.Cluster1[i].CMember;
		string NMember = "";
		char* CM = new char[CMember.length() + 1];
		strcpy(CM, CMember.c_str());
		char* Member;
		Member = strtok(CM, "|");//�ַ������strtok()
		while (Member != NULL)
		{
			string Man = Member;
			if (Man != buf)
			{
				NMember += Man + "|";
			}
			Member = strtok(NULL, "|");
		}
		D.ChangeData(2, "CMember = '" + NMember + "'", "CNum = " + CNum);
		delete[] CM;
	}
	
	return "12#Success";
}

string UserManage::SelecteGroup(string buf)//buf:����������û��˺�|�����˺�;return : 13#�����˺�#Success
{
	char* acc = new char[buf.length() + 1];
	char* num = new char[buf.length() + 1];
	char* Buffer = new char[buf.length() + 1];
	strcpy(Buffer, buf.c_str());
	sscanf(Buffer, "%[^|]|%s", acc, num);
	string Num = num, Acc = acc;
	delete[] acc;
	delete[] num;
	delete[] Buffer;

	string Member1 = Acc + "|" + Num;
	string Member2 = Num + "|" + Acc;

	D.DeleteData(2, "CMember='" + Member1 + "' or CMember='" + Member2 + "'");
	return "13#" + Num + "#Success";
}
