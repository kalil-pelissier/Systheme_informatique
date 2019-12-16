#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <strings.h>
#include <unistd.h>
#define PORT 12345
int sock, socket2, lg;
char mess[80];
struct sockaddr_in local; // champs d entete local
struct sockaddr_in distant; // champs d entete distant
void creer_socket()
{
// preparation des champs d entete
bzero(&local, sizeof(local));
local.sin_family = AF_INET;
local.sin_port = htons(PORT);
local.sin_addr.s_addr = INADDR_ANY;
bzero(&(local.sin_zero),8);
// mise a zero de la zone adresse
// famille d adresse internet
// numero de port
// types d adresses prises en charge
// fin de remplissage
lg = sizeof(struct sockaddr_in);
sock=socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == -1) {
		perror("Problème création socket");exit(1);
	}
	if (bind(sock, (struct sockaddr *)&local, sizeof(struct sockaddr)) == -1){
		perror("Problème de liaison de la socket à une adresse");exit(1);
	}
}
main()
{
// creation socket
creer_socket();

  if (listen(sock, 5) == -1){
    perror("Problème avec listen");exit(1);
  }

// boucle sans fin pour la gestion des connexions
while(1)
{ // attente connexion client
printf ("En attente d un client\n");
socket2=accept(sock, (struct sockaddr *)&distant, &lg);
if (socket2 == -1) {
	perror("Problème avec accept");exit(1);
}
printf ("client connecte \n");
strcpy(mess,"");
while (strncmp(mess,"fin",3)!=0)
{ read(socket2,mess,80);
printf ("le client me dit %s \n",mess);
write(socket2, "message recu !",80);
}
close(socket2);
// on lui ferme la socket}
}