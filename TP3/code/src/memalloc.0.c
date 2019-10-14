#include <memalloc.h>
#include <stdio.h>

typedef struct bloc_entete
{
 size_t taille; //taille du bloc utilisateur
 unsigned short libre : 1 ; //drapeau de 1 bit qui indique si le bloc est libre ou utilise: 1 libre, 0 utilise.
} bloc_entete ;

void bloc_info0(void* ptr)
{
 bloc_entete* bloc_total= (bloc_entete*) ((char*)ptr - ENTETE_SIZE);
 printf("pointeur bloc %p \n, pointeur donnees %p \n, taille totale: %d, \nbloc libre ?: %d\n",
	bloc_total,
	ptr,
	(int) bloc_total->taille,
	bloc_total->libre);
}
//todo
void* myalloc0(size_t t)
{
  int taille = ALIGN(t)+ENTETE_SIZE;
  void* ptr = sbrk(taille);
  
  //initialisation de mon entet
  bloc_entete* entete = (bloc_entete*)ptr;
  entete->libre = 0;
  entete->taille = taille;

  ptr = ptr+ENTETE_SIZE;
  return ptr;
};

//todo
void myfree0(void* ptr)
{
	void* temp = ptr-ENTETE_SIZE;
	bloc_entete* t = (bloc_entete*)temp;
	t->libre = 1;
}


