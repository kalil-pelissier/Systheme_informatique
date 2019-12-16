#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXLIGNE 80
#define MAXCMD 120
#define MAXRESULT 80
#define USAGE "usage: %s fichier"




int main(int argc, char** argv)
{
	char ligne[MAXLIGNE];
	char cmd[MAXLIGNE];
	char resultat[MAXRESULT];

	if(argc < 2)
	{
	printf(USAGE,argv[0]);
	return(1);
	}

	FILE* input = fopen(argv[1],"r");

	if(input == NULL)
	{
	printf("Erreur ouverture du fichier %s",argv[1]);
	exit(1);
	}

	while ( fgets ( ligne, MAXLIGNE, input ) != NULL )
	{
	//ici nous retirons le retour a la ligne
		 if( ligne[strlen(ligne)-1] == '\n')
		 {
		 ligne[strlen(ligne)-1] = '\0';
		 }
	sprintf(cmd,"echo '%s' | sort",ligne);
	FILE* pipe_fp;
	/* creation d un tube process pour utiliser la commande sort*/
		if (( pipe_fp = popen(cmd, "r")) == NULL)
		{
		perror("popen");
		exit(1);
		}

		if ( fgets(resultat, MAXRESULT, pipe_fp) == NULL){
		perror("fgets");
		exit(1);
		}
	pclose(pipe_fp);
	printf("%s",resultat);
	}
	return(0);
}