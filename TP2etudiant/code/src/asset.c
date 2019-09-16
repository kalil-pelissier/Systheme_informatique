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
//TODO
return KO;
};
