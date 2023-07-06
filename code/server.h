#ifndef __SERVER_H__
#define __SERVER_H__
int initServer(short port);
//run the server
int runServer(const char* home);
//deinit server
void deinitServer(void);

#endif
