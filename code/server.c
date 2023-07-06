#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "signals.h"
#include "socket.h"
#include "client.h"
#include "server.h"

int initServer(short port){
//initialise server
	initSignals();
	//initialise socket
	if(initSocket(port)==-1){
		return -1;
	}
	return 0;
}
//run the server
int runServer(const char* home){
	for(;;){
	int conn = acceptClient();
	if(conn == -1){
		perror("conn");
		return -1;
		}
	//To ensure that the lifespan of the structure is long enough.
	CA* ca = malloc(sizeof(CA));
	ca -> conn = conn;
	ca -> home = home;
	
	pthread_t tid;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
	int error = pthread_create(&tid,&attr,client,ca);
	if(error){
		fprintf(stderr,"pthread_create:%s\n",strerror(error));
		return -1;
		}
	}
}

void deinitServer(void){
	deinitSocket();
}


