#include <util.h>
#include <datamgt.h>

#include <string.h>
#include <limits.h>
//allocation de la memoire de taille size; le resulat est dans r
int _malloc(struct age_t *a, size_t size, void ** r);
//re allocation de la memoire de taille newSize; le resulat est dans r
int _realloc(struct age_t *a, void *ptr, size_t oldSize, size_t newSize, void ** r);
//liberation de la memoire
int _free(struct age_t *a, void *ptr);

//insertion dans l ensemble
int sparseIntegerSet_insert(struct age_t *age, sparseIntegerSet_t *is, unsigned int i){
    //si l entier est plus petit que la capacite alors augmenter celle ci.
    if(i >= is->capacity) 
	{
		unsigned int newCapacity = (i + 1) * 1.5;
		_realloc(age, is->dense, is->capacity * sizeof(unsigned int), newCapacity * sizeof(unsigned int), (void **)&is->dense);
		_realloc(age, is->sparse, is->capacity * sizeof(unsigned int), newCapacity * sizeof(unsigned int), (void **)&is->sparse);
		is->capacity = newCapacity;
	}

	is->dense[is->population] = i;
	is->sparse[i] = is->population;
   	is->population++;
	return OK;

};

//retirer un entier de l'ensemble
int sparseIntegerSet_delete(struct age_t *age, sparseIntegerSet_t *is, unsigned int i){
	
	is->population--;
	is->dense[is->sparse[i]]=is->dense[is->population];
	is->sparse[is->dense[is->population]]=is->sparse[i];
	return OK;
};
//vider completement l ensemble
void sparseIntegerSet_clear(struct age_t *age, sparseIntegerSet_t *is) {

is->population = 0;

};

//demander si l ensemble est vide
int sparseIntegerSet_isEmpty(struct age_t *age, sparseIntegerSet_t *is) {

    	return(is->population==0);
};
	
//retirer la valeur en tete 
unsigned int sparseIntegerSet_pop(struct age_t *age, sparseIntegerSet_t *is){

    	is->population--;
	return OK;

};

void sparseIntegerSet_free(struct age_t *age, sparseIntegerSet_t *is){
	_free(age, is->dense);
	_free(age, is->sparse);
	memset(is, 0, sizeof(*is));
};

int sparseIntegerSet_contains(struct age_t  *age, struct sparseIntegerSet_t *is, unsigned int i) {
    
	return (i==is->dense[is->sparse[i]]);
}
