#include <memalloc.h>
#include <stdio.h>

typedef struct bloc_entete
{
 size_t taille; //taille du bloc utilisateur
 unsigned short libre : 1 ; //drapeau de 1 bit qui indique si le bloc est libre ou utilise: 1 libre, 0 utilise.
} bloc_entete ;

 void* start;
 int initialiser;
 	

void* myalloc1(size_t t){
	//initialisatio de start pour le premier myalloc1
	if(initialiser == 0) {
  	initialiser = 1;
  	start = sbrk(0);
    return myalloc0(t);
 };

 	 void* fin = sbrk(0);
	 void* current = start;

   do{
   		bloc_entete* entete = (bloc_entete*)current;

   		int blocklib = entete->libre; 
    	int blocktaille = entete->taille; 

   		if(blocklib == 1 && blocktaille >= ENTETE_SIZE+t) {
   			current += ENTETE_SIZE;
   			return current;
   		}

   		current += blocktaille;

   }while(current < fin);

   		return myalloc0(t);
}


void myfree1(void* ptr)
{
	void* temp = ptr-ENTETE_SIZE;
	bloc_entete* t = (bloc_entete*)temp;
	t->libre = 1;
}

void bloc_info1(void* ptr){
    bloc_info0(ptr);
}
