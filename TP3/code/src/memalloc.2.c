#include <age.h>
#include <memalloc.h>


typedef struct bloc_entete
{
 size_t taille; //taille du bloc utilisateur
 struct bloc_entete* next; //next block free
 struct bloc_entete* previous;//previous bloc
} bloc_entete ;

struct bloc_entete* firstFree;
void* teteFree;
int init = 0;

void bloc_info2(void* ptr){
    bloc_info1(ptr);
}

struct bloc_entete* getLibre(size_t t){
	bloctete* i = firstFree; 
	while(i->next =/ null && i->taille < t){
		i = i->next;
	}
	if(i->taille >= t){
		return i;
	}else{
		return null;
	}
}

//todo
void* myalloc2(size_t t)
{
	struct bloc_entete* recycle = getLibre(t);
	{
		
	};
	if(init == 0 || recycle == null){
		int taille = ALIGN(t);
	  	void* ptr = sbrk(taille);
	  
	  	//initialisation de mon entet
	  	bloc_entete* entete = (bloc_entete*)ptr;
	  	entete->taille = taille;
	  	entete->next = null;
	  	entete->previous = null;

	 	ptr = ptr+ENTETE_SIZE;
	  	return ptr;
	}
	else if(recycle =/ null)
	{
		return (void*)recycle+ENTETE_SIZE;	
	}
};


//todo
void myfree2(void* ptr)
{
	if(init == 0){
		firstFree = ptr-ENENTETE_SIZE;  
		teteFree = firstFree;
		init = 1;
		//on verra plus tard si on utilise la variable libre ou pas
	}else{
		void* current = ptr-ENENTETE_SIZE;
		bloc_entete* bloc = (bloc_entete*)current;
		bloc_entete* bloctete = (bloc_entete*)teteFree;
		bloc->previous = bloctete;
		bloctete->next = bloc;
		teteFree = current; 
	}
}
