#include <util.h>
#include <datamgt.h>

#include <string.h>
#include <limits.h>

int _malloc(struct age_t *a, size_t size, void ** r);
int _realloc(struct age_t *a, void *ptr, size_t oldSize, size_t newSize, void ** r);
int _free(struct age_t *a, void *ptr);

//insertion dans l ensemble
int sparseIntegerSet_insert(struct age_t *age, sparseIntegerSet_t *is, unsigned int i){
    //si l entier est plus petit que la capacite alors augmenter celle ci
    if(i >= is->capacity) 
	{
		unsigned int newCapacity = (i + 1) * 1.5;
		_realloc(age, is->dense, is->capacity * sizeof(unsigned int), newCapacity * sizeof(unsigned int), (void **)&is->dense);
		_realloc(age, is->sparse, is->capacity * sizeof(unsigned int), newCapacity * sizeof(unsigned int), (void **)&is->sparse);
		is->capacity = newCapacity;
	}
    // essayer d avoir l index de notre entier
    unsigned int a = is->sparse[i];
    unsigned int n = is->population;
    //si l entier i n est pas deja present dans l ensemble alors nous devons le rajouter
    if( !( a < n && is->dense[a] == i ) )
    {
        is->sparse[i] = n; // mettre l index de l entier i dans sparse
		is->dense[n] = i; // mettre la valeur i a l index n 
		is->population = n + 1; // augmenter la taille de la population
		return OK;
    }
    return KO;
};

//retirer un entier de l'ensemble
int sparseIntegerSet_delete(struct age_t *age, sparseIntegerSet_t *is, unsigned int i){
    //pour retirer l entier il suffit de le remplacer par la derniere insertion

    //verification que l entier a retirer fait partie de l intervalle et que l ensemble n est pas vide
    if(i >= is->capacity || is->population == 0) {
		return KO;
	}
	unsigned int a = is->sparse[i];
	unsigned int n = is->population - 1; //index du dernier entier insere
	
    if(a <= n || is->dense[a] == i) 
    {
		unsigned int e = is->dense[n];
		is->population = n; // decrement de la population de l ensemble
		is->dense[n] = e; // replacement de la valeur de i
		is->sparse[e] = a;
		return OK;
	}
	return KO;
};
//vider completement l ensemble
void sparseIntegerSet_clear(struct age_t *age, sparseIntegerSet_t *is) {
is->population = 0;
};

//demander si l ensemble est vide
int sparseIntegerSet_isEmpty(struct age_t *age, sparseIntegerSet_t *is) {
return is->population == 0;
};
//retirer la valeur en tete 
unsigned int sparseIntegerSet_pop(struct age_t *age, sparseIntegerSet_t *is){
if(is->population >= 1) {
		return is->dense[--is->population];
	}
	return UINT_MAX;
};

void sparseIntegerSet_free(struct age_t *age, sparseIntegerSet_t *is){
	_free(age, is->dense);
	_free(age, is->sparse);
	memset(is, 0, sizeof(*is));
};

int sparseIntegerSet_contains(struct age_t  *age, struct sparseIntegerSet_t *is, unsigned int i) {
	if(i >= is->capacity) {
		return 0;
	}
	unsigned int a = is->sparse[i];
	unsigned int n = is->population;
	return a < n && is->dense[a] == i;
}
