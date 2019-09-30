#include <memalloc.h>

typedef struct bloc_entete
{
 size_t taille; //taille du bloc utilisateur
 unsigned short libre : 1 ; //drapeau de 1 bit qui indique si le bloc est libre ou utilise: 1 libre, 0 utilise.
} bloc_entete ;



//TODO
void* myalloc1(size_t t)
{
  return NULL;
};

//TODO
void myfree1(void* ptr)
{
  return;
}

//TODO
void blocinfo1(void* ptr){
  
}
