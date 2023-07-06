#ifndef __CLIENT_H__
#define __CLIENT_H__
//struct to transfer two strings(so real path is obtained)
typedef struct clientArgs{
	int conn;
	const char* home;//the local path
}CA;

//thread function
void* client(void* arg);

#endif
