#include<unistd.h>
#include <sys/syscall.h>
#include <stdio.h>
#define __USE_GNU//must be above string.h so you can declare the special struct functions 
#include <string.h>
#include <time.h>
#include "http.h"

int parseRequest(const char* req, HTTPS_REQUEST* hreq){
	//sscanf is taking data from a string literal with spacing used to distinguish between elements
	sscanf(req,"%s%s%s",hreq->method, hreq->path,hreq->protocol);
	// strstr find a smaller string in the larger string. It will return the index of the smaller str in the bigger str. strcasestr ignores upper/lower cases. 
	char* con = strcasestr(req,"connection");
	if(con){
		sscanf(con,"%*s%s",hreq->connection);//Stores the second smaller string into the structure
		printf("%d.%ld > [%s][%s][%s]\n",getpid(), 
			syscall(SYS_gettid),
			(hreq->path), 
			(hreq->protocol),
			(hreq->connection));	
		//To find if the request starts with GET.
		if(strcasecmp(hreq->method, "get")){
			syscall(SYS_gettid);
			return -1;
		}
		// only when the first and second conditions are true does the following get printed.
		if(strcasecmp(hreq->protocol, "http/1.0")&& strcasecmp(hreq->protocol,"http/1.1")){
			printf("%d.%ld> ineffective protocol.PLease use https 1.0/1.1 protocol\n",getpid(),syscall(SYS_gettid));
			return -1;
		}
	return 0;
	}

}

int constructHead(const HTTPS_RESPONSE* hres, char* head){
	char dataTime[32];
	time_t now = time(NULL);
	//%a means abbrevation of weekdays. Convert it to actual struct using gmtime()
	strftime(dataTime, sizeof(dataTime),"%a %d %b %Y %T",gmtime(&now));	
	sprintf(head,"%s %d %s\r\n"
		"Server: WinnieisnotapoohServer\r\n"
		"Date: %s\r\n"
		"Content-Type:%s\r\n"
		"Content-Length: %ld\r\n"
		"Connection: %s\r\n\r\n",
		(hres->protocol),
		(hres->status),
		(hres->desc),
		dataTime,
		(hres->type),
		(hres-> length),
		(hres->connection)
	);
	return 0;
}
