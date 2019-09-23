#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
	

void mytee(char path) {
	char buf; 
	int rd, fd;
	fd = open(&path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if(fd<0) {
		printf("%d\n", fd);
		exit(1);
	}
	do{
		rd = read(0, &buf, sizeof(char));
		write(fd, &buf, sizeof(char));
	} while (rd == sizeof(char));
}



int main(int argc, char argv[]) {

mytee(argv[1]);

}

