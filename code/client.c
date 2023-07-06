#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> //close and access
#include <sys/syscall.h>
#include <string.h>
#include <fcntl.h> //open
#include <sys/stat.h> //statistics, fifo
#include "http.h"
#include "socket.h"
#include "client.h"
#include "resource.h"

//client side threads

void* client(void* arg){
	//arg --> &CA
	CA* ca = (CA*)arg;
	printf("%d.%ld > client thread begins\n",getpid(), syscall(SYS_gettid));
	for(;;){
		printf("%d.%ld > receiving requests\n",getpid(),syscall(SYS_gettid));
		char* req = recvRequest(ca->conn);
		if(req == NULL){
			break;
		}
		printf("%d.%ld > requesting for address\n",getpid(), syscall(SYS_gettid));
		printf("%d.%ld > parsing request",getpid(), syscall(SYS_gettid));
		HTTPS_REQUEST hreq = {};//to store structure
		if(parseRequest(req, &hreq)==-1){
			free(req);//free mem space allocated to the address 
			break;
		}
	 	printf("%d.%ld parsing successful\n",getpid(), syscall(SYS_gettid));
		free(req);//need to free as rlvnt info already saved inside req
		//resource path
		char root[PATH_MAX +1];
		//real path
		char path[PATH_MAX +1];
		
		strcpy(root,ca->home);
		if(root[strlen(root)-1]=='/'){
			root[strlen(root)-1]= '\0';//strcat wouldnt cat after \0
		}
		strcpy(path,root);//To first add the home address before adding path
		strcat(path,hreq.path);
		//if the path is /, provide index.html
		if(strcmp(hreq.path,"/")==0){
			strcat(path, "index.html");
			//so that the path is now the document itself
		}				
		//Relying to the client.This is the most likely case.
		HTTPS_RESPONSE hres = {"HTTP/1.1",200,"OK","text/html"};
		if(searchResource(path)==-1){
			hres.status = 404;
			//You should copy it inside and not let it equal directly
			strcpy(hres.desc, "NOT FOUND");
			strcpy(path,root);
			//~/linuxstudying/day19/project/home/404.html
			strcat(path, "/404.html");
		}else if(identifyType(path,hres.type)==-1){
				hres.status = 404;
				strcpy(hres.desc, "NOT FOUND");
				strcpy(path,root);
				strcat(path, "./404.html");
			}
		//to retrieve the size of the file using stat.
		struct stat s;
		if(stat(path,&s)==-1){break;}
		hres.length = s.st_size;
		
		if(strlen(hreq.connection)){
			strcpy(hres.connection,hreq.connection);
		}else if(strcasecmp(hreq.protocol,"https/1.1")==-1){
			strcpy(hres.connection,"close");
		}else{
			strcpy(hres.connection,"keep-alive");
		}
		char head[1024]={};
		if(constructHead(&hres,head)==-1){
			break;
		}
		printf("%d.%ld > replying to request:\n%s",getpid(),syscall(SYS_gettid),head);
		//sending over the reply
		if(sendHead(ca ->conn, head)==-1){
			break;
		}	
		printf("sendHead: sent to client\n");
		if(sendBody(ca ->conn, path)==-1){
			break;
		}	
		printf("sendBody: sent to client\n");
		if(strcasecmp(hres.connection, "close")){
			break;
		}	
	}	
	close(ca->conn);
	free(ca);
	printf("%d.%ld> client thread ends\n",getpid(),syscall(SYS_gettid));
	return NULL;
}


