#include <age.h>

/*
 * Gestion des blocs avec une liste chainee
 */
typedef struct bloc_entete
{
  size_t taille; //taille du bloc utilisateur
  struct bloc_entete* suivant_ptr ;    // pointeur sur le bloc suivant dans la liste
  struct bloc_entete* precedent_ptr ;    // pointeur sur le bloc precedent dans la liste
  
} bloc_entete ;

//TODO
void* myalloc2(size_t t)
{
return NULL;
};
//TODO
void blocinfo2(void* ptr){

}
//TODO
void myfree2(void* ptr)
{

  return;
}
