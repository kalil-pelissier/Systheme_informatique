//code de ecrivain
#include <stdio.h>
#include <stdlib.h>
#define FIFO_PATH
"/tmp/MAFIFO"
int main(int argc, char *argv[])
{
FILE *fp;
if ( argc != 2 ) {
printf("USAGE: %s [string]\n",argv[0]);
exit(1);
}
if((fp = fopen(FIFO_PATH, "w")) == NULL) {
perror("fopen");
exit(1);
}
fputs(argv[1], fp);
fclose(fp);
return(0);
}