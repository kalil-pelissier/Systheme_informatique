#ifndef __DATA_MGT_H__
#define __DATA_MGT_H__

#include <stddef.h>
#include <stdbool.h>

//nous allons developper un moteur de jeu, ceci est une structure qui le represente
//mais pour cette seance nous n allons pas la definir
typedef struct age_t age_t;

//fonctions pour gerer la memoire en utilisant notre moteur de jeu
int _malloc(struct age_t *a, size_t size, void ** r);
int _realloc(struct age_t *a, void *ptr, size_t oldSize, size_t newSize, void ** r);
int _free(struct age_t *a, void *ptr);


// ============================================================================
// Ensemble creux d entiers 
typedef struct sparseIntegerSet_t {
	unsigned int *dense;
	unsigned int *sparse;
	unsigned int population;
	unsigned int capacity;
} sparseIntegerSet_t;

//insertion dans l ensemble
int sparseIntegerSet_insert(struct age_t *age, sparseIntegerSet_t *is, unsigned int i);
// test si une valeur est dans l ensemble
int sparseIntegerSet_contains(struct age_t  *age, struct sparseIntegerSet_t *is, unsigned int i);
//retirer un entier de l'ensemble
int sparseIntegerSet_delete(struct age_t *age, sparseIntegerSet_t *is, unsigned int i);
//vider completement l ensemble
void sparseIntegerSet_clear(struct age_t *age, sparseIntegerSet_t *is) ;
//demander si l ensemble est vide
int sparseIntegerSet_isEmpty(struct age_t *age, sparseIntegerSet_t *is) ;
//retirer la valeur en tete 
unsigned int sparseIntegerSet_pop(struct age_t *age, sparseIntegerSet_t *is);
//
void sparseIntegerSet_free(struct age_t *age, sparseIntegerSet_t *is);


#endif
