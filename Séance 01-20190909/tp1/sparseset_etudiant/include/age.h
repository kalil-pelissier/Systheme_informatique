#ifndef __AGE__H
#define __AGE__H

#define AGE_VERSION "0.0.1"

#include <stddef.h>


// definition des erreurs dans une enumeration
enum {
	AGE_ERROR_NONE,
	AGE_ERROR_OUT_OF_MEMORY,
	AGE_ERROR_INVALID_VALUE,
	AGE_ERROR_INVALID_OPERATION,
	AGE_ERROR_FULL_COMPONENT
};



// singleton pour le moteur de jeu
struct age_t;

int age_initialize(struct age_t *);
int age_process(struct age_t *age, float delta);
int allocate_age(void *(*malloc)(size_t), void (*free)(void *), struct age_t ** r);
int age_free(struct age_t *age);

typedef struct age_t {
	void *(*malloc)(size_t);
	void (*free)(void *);
	int initialized;
} age_t;


#endif
// code du moteur de jeu en entite systeme inspire du projet diana disponible sur https://github.com/discoloda/Diana/