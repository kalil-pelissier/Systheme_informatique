#ifndef __DATA_MGT_H__
#define __DATA_MGT_H__

#include <stddef.h>
#include <stdbool.h>

typedef struct age_t age_t;
int _malloc(struct age_t *a, size_t size, void ** r);
int _realloc(struct age_t *a, void *ptr, size_t oldSize, size_t newSize, void ** r);
int _free(struct age_t *a, void *ptr);
int _strdup(age_t *age, const char *s, char ** r);

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
void sparseIntegerSet_free(struct age_t *age, sparseIntegerSet_t *is);

// ============================================================================
// Fonction utiles pour la gestion des bits 
//mette un 1 au bit d index nbit dans bytes
int bits_set(unsigned char *bytes, unsigned int nbit);
//mette un 0 au bit d index nbit dans bytes
int bits_clear(unsigned char *bytes, unsigned int nbit);

//demande si le bit est a 1 a l index nbit
int bits_isSet(unsigned char *bytes, unsigned int nbit) ;

// ============================================================================
// Ensemble dense d entiers 


typedef struct denseIntegerSet_t {
    unsigned char *bytes;
	unsigned int capacity;
} denseIntegerSet_t;

//insertion d un element
unsigned int denseIntegerSet_insert(age_t *age, denseIntegerSet_t *is, unsigned int i);

//retirer un element de l ensemble
unsigned int denseIntegerSet_delete(age_t *age, denseIntegerSet_t *is, unsigned int i); 
//librere la structure
void denseIntegerSet_free(age_t *age, denseIntegerSet_t *is);

// ============================================================================



// List
typedef struct {
  int num_items;
  int num_slots;
  void** items;
} list_t;

list_t* list_new(age_t *age);

void list_push_back(age_t *age, list_t* l, void* item);
void* list_pop_back(age_t *age,list_t* l);
void* list_pop_at(age_t *age,list_t* l, int index);
void list_remove(age_t *age,list_t* l, void* item);
void* list_get(list_t* l, int index);
void list_set(list_t* l, int index, void* item);
bool list_is_empty(list_t* l);
void list_delete(age_t *age,list_t* l);
void list_clear(age_t *age,list_t* l);
void list_delete_with(age_t *age,list_t* l, void func(void*));
void list_clear_with(age_t *age,list_t* l, void func(void*));


/* Buckets (Linked List) */
typedef struct bucket {
  char* key;
  void* item;
  struct bucket* next;
} bucket_t ;

struct bucket* bucket_new(age_t *age,char* string, void* item);
void bucket_map(struct bucket* b, void func(void*) );
void bucket_filter_map(struct bucket* b, int  filter(void*) , void func(void*) );
void bucket_delete_with(age_t *age,struct bucket* b, void func(void*) );
void bucket_delete_recursive(age_t *age,struct bucket* b);
void bucket_print(struct bucket* b);

/* dict */
typedef struct {
  int size;
  struct bucket** buckets;
} dict ;

dict* dict_new(age_t *age,int size);
void dict_delete(age_t *age,dict* d);
bool dict_contains(dict* d, char* string);
void* dict_get(dict* d, char* string);
void dict_set(age_t * e,dict* d, char* string, void* item);
void dict_remove_with(age_t *age, dict* d, char* string, void func(void*));
void dict_map(dict* d, void func(void*));
void dict_filter_map(dict* d, int filter(void*) , void func(void*) );
void dict_print(dict* d);
char* dict_find(dict* d, void* item);

//Dict and List from https://github.com/orangeduck/Corange


#endif