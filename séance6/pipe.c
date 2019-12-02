#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
main()
{
	int fd[2];
	pipe(fd);
}
