#include <stdio.h>
#include <stdlib.h>
#include "server.h"
#include <arpa/inet.h>
int main(int argc, char* argv[]){
	//./a.out 8090
	if(initServer(argc < 2 ? 80: atoi(argv[1]))==-1){
	return -1;
	}
	if(runServer(argc<3?"../home":argv[2])==-1){
		return -1;
	}
	deinitServer();
return 0;
}
