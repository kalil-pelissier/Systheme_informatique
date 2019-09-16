#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

void mytee(char* path) {
	int fd = open(path, O_RDONLY, 0644);
	char buf; 
	int rd;
	do
	{
		rd = read(fd, &buf, sizeof(char));
		write(1, &buf, sizeof(char));

	} while (rd == sizeof(char));
	close(fd); 
}

int main(int argc, char argv[]) {

mytee(argv[1]);

}

