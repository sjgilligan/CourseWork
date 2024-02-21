#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
	FILE * pFile;
    	pFile = fopen("/dev/mytimer","r+");
    	if (pFile==NULL) {
        	fputs("mytimer module isn't loaded\n",stderr);
        	return -1;
    	}
	
	int delay;
	char message[256];
	int count;
	if(strcmp(argv[1],"-l") == 0 && argc == 2){
		//printf("list");
		fputs("l",pFile);
	}else if(strcmp(argv[1],"-s") == 0 && argc == 4){
		delay = atoi(argv[2]);
		strncpy(message,argv[3],sizeof(message));
		//printf("message = %s, d = %d\n",message,delay);
		char str[500];
		sprintf(str,"s %d %s",delay,message); //format buffer
		fputs(str,pFile);
	}else if(strcmp(argv[1], "-m") == 0 && argc == 3){
		count = atoi(argv[2]);
		//printf("count = %d\n",count);
		char str[10];
		sprintf(str,"m %d",count);
		fputs(str,pFile);
	}else{
		printf("error");
	}
	
	fclose(pFile);
    return 0;
}
