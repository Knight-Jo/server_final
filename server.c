#include "server.h" 


UserManage UserManage ; 
GroupManage GroupManage ;

static int DEBUG = 1 ; 
int listenfd , connectfd ; 
struct sockaddr_in client, server ; 

int send_q_head, send_q_tail ; 
struct send_property send_queue[MAX_SEND_QUEUE_SIZE] ; 
//lock for the queue 
pthread_mutex_t queue_lock ; 

struct client_property client_prop[MAX_CLIENT_COUNT] ; 

// 初始化服务器 
// initialize data structures :memssage queue  client_prop->client_fd 
// multithread lock 
// network( socket)   
void init_server()
{
	 
	send_q_head = send_q_tail = 0 ;
	memset(send_queue,  0, sizeof(send_queue ) ) ; 
	memset(client_prop, 0, sizeof(client_prop) ) ; 
	for(int i=0; i < MAX_CLIENT_COUNT; i++) 
		client_prop[i].client_fd = -1; 
	
	pthread_mutex_init(&queue_lock, NULL) ;  
	init_socket() ;  
	if(DEBUG) Log("init_server sucessful!" ) ; 
}

void init_socket()
{
	if( (listenfd = socket(AF_INET,SOCK_STREAM,0) ) <0)
		handle_error("socket") ; 
	int opt = SO_REUSEADDR ; 
	setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt) ) ; 
	bzero( &server, sizeof(server) ) ; 
	server.sin_family	   = AF_INET ; 
	server.sin_port		   = htons(LISTEN_PORT) ; 
	server.sin_addr.s_addr = htonl(INADDR_ANY) ; 
	if (bind(listenfd, (struct sockaddr *)(&server) , sizeof(server)) <0 )
		handle_error("bind") ; 
	if (listen(listenfd ,MAX_CLIENT_COUNT) < 0) 
		handle_error("listen") ; 
}
 /* 监听线程
  *
  * */
void *listen_thread_function(void *arg) 
{
	socklen_t sin_size = sizeof(struct sockaddr_in) ; 
	while(1)
	{

		if((connectfd=accept(listenfd,(struct sockaddr *) &client,
						&sin_size)) < 0) 
			handle_error("accept ()") ; 
		
		if(DEBUG){
		   	Log   ("connection from: ") ; 
			printf("				%s\n" , inet_ntoa(client.sin_addr) ) ; 
		}
		add_client(connectfd , client) ; 
	}
} 
 /* 一个新的客户端连接到服务器
  * connectfd 链接的用户的文件描述符
  * client	  用户套接字 */
void add_client(int connectfd, struct sockaddr_in addr ) 
{
	int idx = 0 ; 
	for(idx=0; idx <MAX_CLIENT_COUNT; idx++)
	{
		if(client_prop[idx].client_fd == -1)
			break ;
	}
	assert(client_prop[idx].client_fd == -1) ; 
	
	client_prop[idx].client_fd = connectfd ; 	
	client_prop[idx].addr	   = addr ; 
	pthread_t tid ; 
	pthread_create(&tid, NULL,client_thread_function, &client_prop[idx]) ; 
	if(DEBUG){
		Log   ("create new thread for connect: "	) ;
		printf("				%s\n", inet_ntoa(addr.sin_addr));
	}
}
 /* 接受用户消息的线程
  * *arg  客户端套接字指针
  */
void *client_thread_function(void *arg)
{
	struct client_property *prop = (struct client_property *) arg ; 
	
	/* char buf[BUFFER_SIZE] ; */ 

	char * buf = (char *) malloc ( 2 *BUFFER_SIZE *(sizeof(char) ) ) ; 
	int numbytes = 0 ;  
	while(1)
	{
		printf("recv ......") ; 
		numbytes = recv(prop->client_fd, buf, BUFFER_SIZE, 0) ;
		if( numbytes <=0)
		{
			printf("user %s is offline.\n",inet_ntoa(prop->addr.sin_addr));
			break ;
		} 
		/* buf[numbytes] = '\0' ; */ 
		handle_client_message(prop , buf) ; 
		
		printf(" OK\n" ) ; 
	}
	remove_client(prop) ; 
	pthread_exit(NULL) ; 
}
void remove_client(struct client_property *prop)
{
	close(prop->client_fd) ; 
	prop->client_fd = -1 ; 
	prop->is_online = 0 ; 
	prop->useraccount = 0 ;

}
 /* 处理客户端发来的消息
  * prop 发送信息的客户端套接字，
  * message 客户端发送来的信息 */
void handle_client_message(struct client_property *prop, const char *message)
{
	if(DEBUG)printf("handling message from client %s,message is \n%s\n",
					inet_ntoa(prop->addr.sin_addr), message) ; 
	int typeint ; 
	char msg[BUFFER_SIZE] ; 
	sscanf(message , "%d|%s", &typeint, msg) ; 
	if(typeint ==JOIN_GROUP ){
		join_group( msg, prop) ; 	
	} else if( typeint == CREATE_GROUP){
		create_group(msg, prop) ; 
	} else if( typeint == INVITE_USER) {
		invite_user(msg, prop) ; 
	} else if(typeint == DISSOLVE_GROUP) {
		dissolve_group(msg, prop) ; 
	} else if(typeint == REGISTER_USER) {
		register_user(msg, prop) ; 
	} else if(typeint == SEARCH_USER) {
		search_user(msg, prop) ; 
	} else if(typeint == ADD_USER) {
		add_user(msg, prop);
	} else if(typeint == CHANGE_INFO) {
		change_info(msg, prop) ; 
	} else if(typeint == LOGIN_USER) {
		login_user(msg, prop) ; 
	} else if (typeint == RECV_MSG) {
		recv_msg(msg, prop) ;
	} else if(typeint == SEND_MSG) {
		send_msg(msg, prop) ;
	} else if(typeint == LOGOUT_USER){
		logout_user(msg, prop) ;
	} else if(typeint == DELETE_USER_GROUP){
		delete_user_group(msg, prop) ;
	} else if(typeint == SHOW_GROUP_MEMBERS){
		show_group_members(msg, prop) ; 
	} else {
		printf("sorry ,I do't know\n") ; 
	} 	
	
}
void join_group(const char * msg, struct client_property  *prop)
{
	/* msg : 用户账号|群号 */
	/* write back: 1#账号#y#群号#群名  ELSE 1#账号#n#该群不存在. */
	if(DEBUG)Log("begin: ") ; 
	string ret = GroupManage.Search(msg) ; 
	cout <<"ret " << ret <<endl ; 
	send_by_fd(ret , prop->client_fd) ; 
	if(DEBUG)Log("end  : ") ; 
}	
void create_group(const char * msg, struct client_property  *prop) 
{
	if(DEBUG) Log("begin: ") ; 
	string ret =  GroupManage.CreatGroup(msg) ; 
	cout << "ret " << ret << endl ; 
	send_by_fd(ret , prop->client_fd) ; 
	if(DEBUG) Log("end: " ) ; 
}

void invite_user(const char * msg, struct client_property  *prop) 
{
	/* msg : 用户账号|群号|被邀请人账号 */ 
	/* 写回： 3#账号#y   ELSE 3#账号#n */ 
	if(DEBUG) Log("begin:") ; 
	string ret = GroupManage.Invite(msg) ; 
	cout << "ret " << ret << endl ; 
	send_by_fd(ret , prop->client_fd) ; 
	if(DEBUG) Log("end  :") ; 	
	
}	
void dissolve_group(const char * msg, struct client_property  *prop) 
{
	/* msg : 账号|群号 */
	/* write back: 4#账号#y#群号  ELSE: 4#账号#n#群号 */
	if(DEBUG) Log("begin:") ; 
	string ret = GroupManage.Dissolve(msg) ; 
	cout << "ret " << ret << endl ; 
	send_by_fd(ret , prop->client_fd) ; 
	if(DEBUG) Log("end  :") ; 	

}	
void register_user(const char * msg, struct client_property  *prop) 
{
	/* msg : 昵称|密码 */
	/* 写回： 5#账号 */
	if(DEBUG) Log("begin: " ) ; 
	string ret = UserManage.Register(msg) ; 
	cout << "ret " << ret << endl ; 
	send_by_fd(ret, prop->client_fd) ; 
	if(DEBUG) Log("end: ") ; 

}	
void search_user(const char * msg, struct client_property  *prop) 
{
    /* msg :用户账号 字符串 */ 
	/* 写回: 6#帐号#昵称#登陆状态# 上次下线时间 */
	/* 	    	ELSE 6#The User Does Not  Exist!) */ 
	if(DEBUG) Log("begin: " ) ; 

	string ret = UserManage.SearchUser( msg) ; 
	cout<<"ret = " <<ret <<endl ; 
	send_by_fd(ret, prop->client_fd) ; 

	if(DEBUG) Log("end: ") ; 

}	
void add_user(const char * msg, struct client_property  *prop)
{
	/* msg : 用户账号|想添加好友账号 */
	/* 写回 ： 7#Y#好友账户#好友昵称  ELSE 7#N */ 
	Log("begin: ") ; 

	string ret = UserManage.AddFriend1( msg) ; 
	cout <<"ret = " <<ret <<endl ; 
	send_by_fd(ret , prop->client_fd) ; 

	Log("end: ") ;
}
void change_info(const char * msg, struct client_property  *prop) 
{
	/* msg : 用户账号 |a(b) |修改的内容 */
	/* 写回：8#Success */ 
	Log("begin: " ) ; 
	string ret = UserManage.ChangeData(msg) ; 
	cout<< "ret = " <<ret <<endl ; 
	send_by_fd(ret, prop->client_fd ) ; 
	Log("end: ") ; 


}	
void login_user(const char * msg, struct client_property  *prop) 
{
	/* msg : 用户账号 */
	/* 写回   9#账号#昵称#头像#好友账号1|好友账户2#群号1|群号2| */ 
	/*  else  9#Failed */ 
	/*	else  9#Exist */

	Log("begin: ") ; 
	int account = 0 ; 
	sscanf(msg, "%d", &account) ; 
	char msg_buf[BUFFER_SIZE] ; 
	sprintf(msg_buf,"%s|%d", msg, prop->client_fd) ; 
	prop->is_online = 1 ; 
	prop->useraccount = account ; 
	string msg_str(msg_buf) ; 

	string ret = UserManage.Logging(msg_str) ; 

	cout <<"ret = " <<ret <<endl ;
	send_by_fd(ret, prop->client_fd) ;

	Log("end:	") ; 

}	
void recv_msg(const char * msg, struct client_property *prop) 
{
	/* msg : 发出用户账号|接受群或用户账号|发送时间|消息内容 */
	/* 写回：11#sendfrom账号#sendto#时间#对话内容 */ 
	Log("begin: " ) ; 
	vector<int> sendto ; 
	string ret = UserManage.Receive(msg, sendto ) ; 
	cout <<"here" <<endl ; 
	cout << "ret = " <<ret <<endl ; 
	const char * ret_char = ret.c_str() ; 	

	int typeint ; 
	int sendfrom_acc ; 
	char tmp[BUFFER_SIZE] ;

	int group_user_id; 
	sscanf(msg, "%d|%d|%s" , &sendfrom_acc,	&group_user_id,tmp);

	/* 发出者返回 */
	char send_from_ret[2*BUFFER_SIZE] ; 
	typeint  = 21 ; 
	sprintf(send_from_ret , "%d#%d#%s", 
						typeint,group_user_id ,ret_char ) ; 
	send_by_fd(send_from_ret , prop->client_fd) ; 	

	/* 接受者返回 */
	typeint = 11 ;
	char send_to_ret[2*BUFFER_SIZE] ; 
	if(group_user_id >39999999)
	{

		sprintf(send_to_ret , "%d#%d#%d#%s", 
				typeint, sendfrom_acc, group_user_id, ret_char) ; 
	}else {
		sprintf(send_to_ret , "%d#%d#%d#%s",
				typeint, sendfrom_acc, sendfrom_acc, ret_char) ; 
	}
	for( int account : sendto){
		int fd = get_user_fd(account) ; 
		send_by_fd(send_to_ret, fd) ; 
	} 
	send_by_fd(send_to_ret , prop->client_fd) ;

	cout << "send from ret= " <<send_from_ret <<endl ; 
	cout << "send to   ret= " << send_to_ret  <<endl  ;

	Log("end: ") ; 

}
void send_msg(const char * msg, struct client_property *prop) 
{
}
void logout_user(const char * msg, struct client_property *prop) 
{
	/* msg : 用户账号 */
	/* 写回：12#successful */ 
	Log("begin: " ) ; 
	string ret = UserManage.SelecteAccount(msg) ; 
	cout<< "ret = " <<ret <<endl ; 
	send_by_fd(ret, prop->client_fd ) ; 
	Log("end: ") ; 

}
void delete_user_group(const char * msg, struct client_property *prop) 
{
	/* msg : 用户账号|好友账号/群号 */
	/* 写回：13#好友账号|successful */ 
	Log("begin: " ) ; 
	int account ; 
	char msg_buf[BUFFER_SIZE]  ; 
	sscanf(msg, "%d|%s", &account, msg_buf ) ; 
	string ret ; 
	if( account > 39999999 ){ 
		ret = GroupManage.Dissolve(msg_buf) ; 
	}else{
		ret = UserManage.SelecteGroup(msg_buf) ;
	} 

	cout<< "ret = " <<ret <<endl ; 
	send_by_fd(ret, prop->client_fd ) ; 
	Log("end: ") ; 

}

void show_group_members(const char *msg, struct client_property *prop) 
{
	/* msg : 用户账号|群号 */
	/* 写回：14#账号#群号#群成员昵称*/ 
	Log("begin: " ) ; 
	string ret = GroupManage.ShowMember(msg) ; 
	cout<< "ret = " <<ret <<endl ; 
	send_by_fd(ret, prop->client_fd ) ; 
	Log("end: ") ; 

} 

void send_by_fd(string msg, int fd)
{
	const char * ret = msg.c_str() ; 
	int val = write(fd, ret , msg.size() ) ; 
	if(val < 0) 
	{
		perror("write back error!") ; 
		return ; 
	} 
}

void send_by_account(int account, const char *msg)
{
	pthread_mutex_lock(&queue_lock) ; 

	if((send_q_tail +1) %MAX_SEND_QUEUE_SIZE == send_q_head) 
		return ; 
	char * message = copy_string (msg) ; 
	send_queue[send_q_tail].message = message ; 
	send_queue[send_q_tail].useraccount = account ; 
	send_q_tail = (send_q_tail + 1 ) % MAX_SEND_QUEUE_SIZE ; 

	pthread_mutex_unlock(&queue_lock ) ; 

}

char *copy_string(const char * src)
{
	char *res = (char *) malloc( (strlen(src) +1) *sizeof(char) ) ;
	strcpy(res, src) ; 
	return res ; 
}
/* 保存离线信息
 * */
void save_offline_message(struct send_property msg)
{

}
 /* 从消息队列 send_queue[]中取出消息，发送或者存储消息
  * */
void *send_thread_function (void *arg) 
{
	struct send_property msg_tmp ; 
	char   send_buffer[BUFFER_SIZE] ; 
	while(1)
	{
		pthread_mutex_lock(&queue_lock) ; 

		/* 队列中有消息要发送 */
		if(send_q_head != send_q_tail)
		{
			msg_tmp = send_queue[send_q_head] ; 
			send_q_head = (send_q_head + 1) % MAX_SEND_QUEUE_SIZE ; 

			/* 得到文件描述符 */
			int fd = get_user_fd(msg_tmp.useraccount); 
			
			if( fd != -1 ) {
				if(DEBUG) printf("sending message to online user %d\b",msg_tmp.useraccount) ; 
				memset(send_buffer, 0 , sizeof(send_buffer) ) ; 

				write(fd, send_buffer, sizeof(send_buffer) ) ; 
			} else{
				/*TODO 用户不在线，保存到数据库还是怎么处理？？？ */
				save_offline_message(msg_tmp) ; 
			}
		}else {  
			/* 队列为空不需要发信息，休眠， */
			sleep(1) ;
		} 
		pthread_mutex_unlock(&queue_lock) ; 
	}


}	
 /* 获取用户文件描述符 */
 /* return client_fd
  * return -1 if error  */ 
int get_user_fd(int useraccount) 
{
	int idx=0; 
	for(idx = 0; idx <MAX_CLIENT_COUNT; idx++)
	{
		if(useraccount == client_prop[idx].useraccount )
			return client_prop[idx].client_fd ; 
	} 
	return -1 ; 
}

