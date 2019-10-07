#include <memalloc.h>

typedef struct bloc_entete
{
 size_t taille; //taille du bloc utilisateur
 unsigned short libre : 1 ; //drapeau de 1 bit qui indique si le bloc est libre ou utilise: 1 libre, 0 utilise.
} bloc_entete ;


//todo
void* myalloc1(size_t t)
{
return 0;
};

//todo
void myfree1(void* ptr)
{

  return;
}

void bloc_info1(void* ptr){
    bloc_info0(ptr);
}
