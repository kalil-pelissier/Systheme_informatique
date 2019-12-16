
#include "message.h"

#include <sys/types.h> 
#include <unistd.h>

#include <sys/stat.h>
#include<stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>


void creation_tube_nomme() {
    mkfifo(FIFO_SERVEUR, 0666);
}
int fifo_me;
struct requete_client_serveur* requete;
void recevoir_requete(){
    fifo_me=open(FIFO_SERVEUR, O_RDONLY);
    requete = malloc(sizeof(struct requete_client_serveur ) );
    read(fifo_me,requete,sizeof(struct requete_client_serveur ));
    close(fifo_me);
    printf("recu %s",requete->expression);
}

void envoyer_resultat(){
    char cmd [80];
    char res [80];
    
    sprintf(cmd,TEMPLATE_CMD,requete->expression);
    FILE * in = popen(cmd, "r");
    fread(res,sizeof(char),BUFFER_SIZE,in);
    pclose(in);


    int pid = requete->client_pid;
    char pathname[80];
    sprintf(pathname,PATH_FORMAT,pid);
    int fifo_client=open(pathname, O_WRONLY);
    strcpy(requete->expression,res);
    write(fifo_client,requete,sizeof(struct requete_client_serveur ));
    close(fifo_client);
    close(fifo_me);
    free(requete);
}

void terminer(){

}

int main(int argc, char** argv){

    creation_tube_nomme();
    
    while(1){
    recevoir_requete();
    envoyer_resultat();
    }

    terminer();
    

}