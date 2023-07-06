#include <unistd.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <string.h>
#include "resource.h"
#include "mime.h"

int searchResource(const char* path){
	return access(path,F_OK);
}

int identifyType(const char* path, char* type){
	// /c/io.html finding char from string in the reverse manner
	char* suffix = strrchr(path, '.');
	if(suffix == NULL){
		printf("%d.%ld > ineffective search\n",getpid(),syscall(SYS_gettid));
		return -1;
	}
	for(int i = 0; i<sizeof(s_mime)/sizeof(s_mime[0]);i++ ){
		if(strcasecmp(s_mime[i].suffix, suffix)==0){
			strcpy(type,s_mime[i].type);
			return 0;
		}
	}
	printf("%d.%ld > Could not identify the document.\n",getpid(), syscall(SYS_gettid));
}
