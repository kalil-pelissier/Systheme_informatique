#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <unistd.h>
#include <stdio.h>

void editer(char *path, char* arg) {
	int fd;
	fd = open(path, O_RDWR, 0644);
	if(arg[0]=='s'){
		int i = atoi(arg+1);
		lseek(fd, i, SEEK_SET);
	}
	else if(arg[0]=='l'){
		char buf; 
		int rd;
		int i = atoi(arg+1);
		do{
			rd = read(fd, &buf, sizeof(char));
			write(1, &buf, sizeof(char));
			i--;
		}while(i!=0 && rd==sizeof(char));
	}
	else if(arg[0]=='w'){
		char buf;
		int rd;
		int i=1;
		do{
			write(fd, &arg[i], sizeof(char));
			i++;
		}while(arg[i]!=0);	
	}
}

int main(int argc, char** argv) {
	int i = 2;
	do{
		editer(argv[1], argv[i]);
		i++	;
	}while(argv[i]!=0);
}
