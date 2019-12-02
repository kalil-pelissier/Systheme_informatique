#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#define PIPE2 "/tmp/serveur_fifo"

#define BUFFER_SIZE 20
struct requete_client_serveur {
pid_t client_pid; //PID du Client
char expression[BUFFER_SIZE - 1]; //text de l expression a evaluer.
};

main()
{ 
	pid_t pid = getpid();
	mkfifo("/temp/")
}