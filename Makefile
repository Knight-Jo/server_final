
LIB = -lsqlite3 -lpthread 
CFLAG = -g -Wall 
CC = g++ 
run : server 
	./server

server : main.o object.o 
	g++ -o server main.o server.o UserManage.o databases.o GroupManage.o  $(LIB) 
test_user: object.o
	g++ -o test_user $(CFLAG) UserManage.o databases.o test_user.cpp -lsqlite3


main.o : main.c 
	g++ -c $(CFLAG) main.c 

object.o : server.c UserManage.cpp databases.cpp GroupManage.cpp  
	g++ -c  $(CFLAG) server.c UserManage.cpp databases.cpp GroupManage.cpp

clean : 
	rm -rf *.o server  main
