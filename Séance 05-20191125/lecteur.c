//code du lecteur
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <linux/stat.h>
#define FIFO_PATH "/tmp/MAFIFO"
int main(void)
{
FILE *fp;
char readbuf[80];
while(1)
{
fp = fopen(FIFO_PATH, "r"); // je suis lecteur
fgets(readbuf, 80, fp);
printf("Recu: %s\n", readbuf);
fclose(fp);
}
exit(0);
}