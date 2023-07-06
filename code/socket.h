#ifndef __SOCKET_H__
#define __SOCKET_H__
//initiate the socket
int initSocket(short port);

//accept the requests from clients
//listening socket shoudl be a global var
int acceptClient(void);

//accept the http request and storing the request for the parsing process
char* recvRequest(int conn);//malloc should be best since threads should not affect one another

//send the replies to specific sockets(conns)
//The address should be given for it to access also
int sendHead(int conn, const char* head);
//sending content of the requested file over.You need the path for this.
int sendBody(int conn, const char* path);

//deinit socket
void deinitSocket(void);

#endif

