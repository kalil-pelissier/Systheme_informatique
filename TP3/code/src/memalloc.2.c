#include <age.h>
#include <memalloc.h>


typedef struct bloc_entete
{
 size_t taille; //taille du bloc utilisateur
 void* next; //next block free
 void* previous;//previous block free
 unsigned short libre : 1 ; //drapeau de 1 bit qui indique si le bloc est libre ou utilise: 1 libre, 0 utilise.
} bloc_entete ;

void* pastFree;
void* firstFree;
int init = 0;

void bloc_info2(void* ptr){
    bloc_info1(ptr);
}

//todo
void* myalloc2(size_t t)
{
	void* current = firstFree;
	if(init == 0) {
		return myalloc0(t);
	}
	else if()
};


//todo
void myfree2(void* ptr)
{
	if(init == 0) {
		firstFree = ptr-ENTETE_SIZE;
		currentFree = firstFree;
		init = 1;
		bloc_entete* free = (bloc_entete*)currentFree;
		t->libre = 1;
	}else {
		void* temp = ptr-ENTETE_SIZE;
		bloc_entete* t = (bloc_entete*)temp;
		bloc_entete* f = (bloc_entete*)pastFree; //je récupère l'entete du derrnier free
		t->libre = 1;
		f->next = temp;
		pastFree = temp;
	}
  
}
