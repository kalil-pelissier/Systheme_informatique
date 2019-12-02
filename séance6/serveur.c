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
{ int t1 ,t2;
char message[80];
t1 = open(PIPE1,O_WRONLY);
t2 = open(PIPE2,O_RDONLY);
strcpy(message, "hello, je suis le processus client");
write(t1,message,strlen(message)) ;
read(t2,message,80) ;
printf("le message recu est :%s\n", message);
exit(0);
}