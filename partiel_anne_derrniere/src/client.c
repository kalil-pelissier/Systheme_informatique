#include "message.h"
#include <sys/types.h> 
#include <unistd.h>

#include <sys/stat.h>
#include<stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>


pid_t pid ;
char pathname[80];
int fifo_serveur_fd;
int fifo_me_fd;

void creation_tube_nomme() {
    pid = getpid(); 
    sprintf(pathname,PATH_FORMAT,pid);
    mkfifo(pathname, 0666);
}

void envoyer_expression(){
    fifo_serveur_fd=open(FIFO_SERVEUR, O_WRONLY);
    
    char msg[] = "5+4";
    struct requete_client_serveur* requete = malloc(sizeof(struct requete_client_serveur ) );
    requete->client_pid = pid;
    strcpy(requete->expression, msg);
    write(fifo_serveur_fd,requete, sizeof(struct requete_client_serveur ) );
    close(fifo_serveur_fd);
    free(requete);
}

void recevoir_resultat(){
    char res[80] ;
    fifo_me_fd=open(pathname, O_RDONLY);
    struct requete_client_serveur* requete = malloc(sizeof(struct requete_client_serveur ) );
    read(fifo_me_fd,requete,sizeof(struct requete_client_serveur ));
    close(fifo_me_fd);
    printf("%s",requete->expression);
    free(requete);
};
    
void terminer()
{
    remove( pathname );
}

int main(int argc, char** argv){

    creation_tube_nomme();
    envoyer_expression();
    recevoir_resultat();
    terminer();
    
    return 0;
}