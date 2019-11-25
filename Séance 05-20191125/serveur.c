/************* PROCESSUS CLIENT ***************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#define PIPE2 "/tmp/pipe2"
#define PIPE1 "/tmp/pipe1"
main()
{ int t1 ,t2;
char message[80];
t1 = open(PIPE1,O_RDONLY);
t2 = open(PIPE2,O_WRONLY);
read(t1,message,80) ;
printf("le message recu est :%s\n", message);
strcpy(message, "hello, je suis le processus serveur");
write(t1,message,strlen(message)) ;
exit(0);
}