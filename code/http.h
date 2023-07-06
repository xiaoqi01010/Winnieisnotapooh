#ifndef __HTTP_H__
#define __HTTP_H__
#include <limits.h>
#include <sys/types.h>

#define PATH_MAX 4096
//Could return data but not very desirable. 
typedef struct hhtpRequest{
	char method[32];//request method
	char path[PATH_MAX +1]; //path of the resource
	char protocol[32];//the type of protocol
	char connection[32];//way of connection
}HTTPS_REQUEST;
//parsing the request
//HTTP_REQUEST r
//parseRequest(req,&r);
int parseRequest(const char* req, HTTPS_REQUEST* hreq);

typedef struct hhtpRespond{
	char protocol[32];//protocl
	int status;//status
	char desc[64]; //status code
	char type[64];//Content-Type
	off_t length;//File size
	char connection[32];//Connection status
}HTTPS_RESPONSE;
//need another address to save the various elements entered so it can be sent later.  
int constructHead(const HTTPS_RESPONSE* hres,char* head);

#endif

