#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include "socket.h"
//Only used in the current file
static int socketfd = -1;

// Initialising
int initSocket(short port){
	printf("%d.%ld>creating a socket\n",getpid(),syscall(SYS_gettid));
	socketfd = socket(AF_INET, SOCK_STREAM, 0);
	if(socketfd == -1){
		perror("socket"); 
		return -1;
	}
	printf("%d.%ld>setting up the socket\n",getpid(),syscall(SYS_gettid));
	//gettid to get the thread id of the current running thread of the current running process
	//One server is not allowed to be bound to multiple sockets \
	// process stopped != socket terminated. We can try to reset the address of the socket.
	int on = 1;
	if(setsockopt(socketfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on))==-1){
		perror("setsocket");
		return -1;
	}
	
	struct sockaddr_in ser;
	ser.sin_family = AF_INET;
	ser.sin_port = htons(port);
	ser.sin_addr.s_addr= INADDR_ANY;
	
	printf("%d.%ld>binding to the port\n",getpid(), syscall(SYS_gettid));
	
	if(bind(socketfd, (struct sockaddr*)&ser, sizeof(ser))==-1){
		perror("bind");
		return -1;
	}
	
	printf("%d.%ld> start listening\n",getpid(),syscall(SYS_gettid));
	
	if(listen(socketfd,1024) == -1){
		perror("listen");
		return -1;
	}
	return 0;
}

int acceptClient(void){
	printf("%d.%ld > waiting for the clients\n",getpid(),syscall(SYS_gettid));
	
	struct sockaddr_in cli;
	socklen_t len = sizeof(cli);
	int conn = accept(socketfd, (struct sockaddr*)&cli, &len);
	
	if(conn == -1){
		perror("accept");
		return -1;
	}
	printf("%d.%ld>received %s:%hu client connection\n",getpid(), syscall(SYS_gettid),inet_ntoa(cli.sin_addr),ntohs(cli.sin_port));
	
	return conn;
}//error is identified by stderror(conn) so you need to return the value of conn

char* recvRequest(int conn){
	printf("in recvRequest!\n");
	//Since the size of the request is unkown, you have to prepare a buffer region. 
	char* req = NULL; //start of the address of the request
	ssize_t len = 0;//length of words received	
	for(;;){
		printf("Receiving request!\n");
		//size of the buffer
		char buf[1024]= {};
		//inidicating the size of the string to be received from client sockets. 
		ssize_t size = recv(conn, buf, sizeof(buf)-1, 0);
		printf("Current size of receiving is %ld\n",size);
		if(size == -1){
			perror("recv");
			free(req);// free mem space allocated. 
			return NULL;
		}
		if(size == 0){
			//cant break here because size == 0 may mean that all str from clients have been received.
			//if the string contains \r\n\r\n then the you can leave the for loop. You can also use the size of string to take out the last 4 elements of the string and compare with \r\n\r\n
			printf("%d.%ld >client socket already closed.\n",getpid(), syscall(SYS_gettid)); 
			return NULL; 	
	}
		//Reallocating the mem space dynamically to req. len+size+1 w/ 1 for \0
		req = realloc(req,len+size+1);
		//Copying over the data from buffer into req from the last written position. 
	 	// recall that req is a char* type and req+len is equivalent to copying from (req+len)th position onwards. 
		memcpy(req+len,buf, size+1);
		//updating the length
		len = len + size;
			if(strstr(req,"\r\n\r\n")){
				printf("finished reading request!\n");
				break;
			}
	}
	printf("The request is: %s\n",req);
	return req;
}
//send respond head
int sendHead(int conn, const char* head){
		if(send(conn, head,strlen(head),0)==-1){
			perror("send");
			return -1;	
	}
	return 0;
}	

//send over the content of the document
int sendBody(int conn,const char* path ){
		int fd = open(path, O_RDONLY);
		if(fd == -1){
			perror("open");
			return -1;
		}
	char buf[1024];
	ssize_t len;
	while((len=read(fd,buf, sizeof(buf)-1))>0){
		if(send(conn, buf, len, 0 )==-1){
			perror("send");
			return -1;
			}
		/*(if(len == -1){
			perror("read");
			close(fd);
			return -1;*/
}
	close(fd);
	return 0;
}
//deinit socket
void deinitSocket(void){

}
