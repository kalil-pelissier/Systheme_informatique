#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

void mytee(char* path) {
	char buf; 
	int rd;
	int fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	do
	{
		rd = read(0, &buf, sizeof(char));
		write(fd, &buf, sizeof(char));

	} while (rd == sizeof(char));
	
}

int main(int argc, char argv[]) {

mytee(argv[1]);

}

