#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <age.h>
#include <util.h>

/* //TODO
 Fonction ouvre un fichier d image ASCII avec le format suivant:
 <largeur>
 <longueur>
 <contenu>
 
 et remplit correctement le contenu de la structure sprite_t* dest.
 Cette fonction doit s occuper de la reservation de la memoire du champs sprite_t.data.
  */
int age_asset_load_sprite(age_t* ge, const char* pathname, sprite_t* dest)
{
    int largeur ;
    int longueur;
    const int buffer_size = 128;
    char buffer[buffer_size]; 
    char ch;
    int state = 0; // variable pour gerer la machine a etat de la lecture du fichier
    int index = 0;
    int nbread;
    int desripteur = open(pathname, O_RDONLY);
    if (desripteur < 0)
    {
        return KO;
    }
    nbread = read( desripteur, &ch , sizeof(char) );
    index = 0;
    while (  (nbread != 0) && (ch != EOF)) {
        switch (state)
        {
        case 0:
            {/* en train de lire de la largeur */
            if(ch == '\n'){
                state = 1;
                buffer[index] = '\0';
                index = 0;
                //largeur ici
                largeur = atoi(buffer);
            }
            else
            {
                if(index < buffer_size)
                {
                buffer[index++] = ch;
                }
            }
            break;
            }
        case 1:
            {/* en train de lire la longueur */
            if(ch == '\n'){
                state = 2;
                buffer[index] = '\0';
                index = 0;
                //longeur ici
                longueur = atoi(buffer);
                // ici je reserve effectivement de la memoire
                printf("larg. %d long %d ",largeur,longueur);
                int taille =  longueur * largeur;
                dest->width = largeur;
                dest->height = longueur;
                _malloc(ge,taille*sizeof(char),(void**)&dest->data);
            }
            else{
                if(index < buffer_size)
                {
                buffer[index++] = ch;
                }
            }
            break;
            }
        case 2:
            {
            /* en train de lire le contenu */
            dest->data[index++] = ch;
            break;
            }

        default:
            break;
        }
    nbread = read(desripteur,&ch,sizeof(char));
       
    }
return OK;
};