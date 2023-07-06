#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/syscall.h>
#include "signals.h"

int initSignals(void){
	printf("%d.%ld > ignore most signals\n",getpid(), syscall(SYS_gettid));
	for(int num = SIGHUP; num<= SIGRTMAX;num++){
		if(num != SIGINT && num != SIGTERM){
			signal(num,SIG_IGN);
		}
	}
	return 0;
}

