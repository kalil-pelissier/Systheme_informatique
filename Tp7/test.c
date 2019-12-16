#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define USAGE "usage: %s <fichier des expressions>"
#define MAXLIGNE 80
#define MAXCMD 120

int main(int argc, char** argv)
{
	char ligne[MAXLIGNE];
	char cmd[MAXLIGNE];

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
	sprintf(cmd,"echo '%s' | bc",ligne);
	printf("%s\n",cmd);
	}

	return(0);
}