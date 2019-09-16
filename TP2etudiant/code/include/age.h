#ifndef __AGE__H
#define __AGE__H

#define AGE_VERSION "0.0.1"

#include <stddef.h>
#include <datamgt.h>
#include <sprite.h>

// definition des erreurs dans une enumeration
enum {
	AGE_ERROR_NONE,
	AGE_ERROR_OUT_OF_MEMORY,
	AGE_ERROR_INVALID_VALUE,
	AGE_ERROR_INVALID_OPERATION,
	AGE_ERROR_FULL_COMPONENT
};

// component flags
#define AGE_COMPONENT_INDEXED_BIT  1
#define AGE_COMPONENT_MULTIPLE_BIT 2
#define AGE_COMPONENT_LIMITED_BIT  4

#define AGE_COMPONENT_FLAG_INLINE     0
#define AGE_COMPONENT_FLAG_INDEXED    AGE_COMPONENT_INDEXED_BIT
#define AGE_COMPONENT_FLAG_MULTIPLE   (AGE_COMPONENT_INDEXED_BIT | AGE_COMPONENT_MULTIPLE_BIT)
#define AGE_COMPONENT_FLAG_LIMITED(X) (AGE_COMPONENT_INDEXED_BIT | AGE_COMPONENT_LIMITED_BIT | ((X) << 3))

// system flags
#define AGE_SYSTEM_PASSIVE_BIT 1

#define AGE_SYSTEM_FLAG_NORMAL  0
#define AGE_SYSTEM_FLAG_PASSIVE AGE_SYSTEM_PASSIVE_BIT

// manager flags
#define AGE_MANAGER_FLAG_NORMAL  0

// les evenements concernant une entite
enum {
	AGE_ENTITY_ADDED,
	AGE_ENTITY_ENABLED,
	AGE_ENTITY_DISABLED,
	AGE_ENTITY_DELETED
};

// assets
#define AGE_MAX_SPRITES 256



typedef struct component_t {
	const char *name;
	size_t size;
	size_t offset;
	unsigned int flags;
	void **data;
    sparseIntegerSet_t freeDataIndexes;
	unsigned int nextDataIndex;
} component_t;

typedef struct componentBag_t {
	unsigned int count;
	unsigned int *indexes;
} componentBag_t;

void component_free(age_t *age, component_t *component) ;


typedef struct _system {
	const char *name;
	unsigned int flags;
	void *userData;
	void (*starting)(age_t *, void *user_data);
	void (*process)(age_t *, void *user_data, unsigned int entity, float delta);
	void (*ending)(age_t *, void *user_data);
	void (*subscribed)(age_t *, void *user_data, unsigned int entity);
	void (*unsubscribed)(age_t *, void *user_data, unsigned int entity);
	sparseIntegerSet_t watch;
	sparseIntegerSet_t exclude;
	denseIntegerSet_t entities;
} system_t ;

void system_free(struct age_t *age, system_t *system) ;

//Manager
typedef struct manager_t {
	const char *name;
	unsigned int flags;
	void *userData;
	void (*added)(age_t* age, void *userData, unsigned int entity);
	void (*enabled)(age_t* age, void *userData, unsigned int entity);
	void (*disabled)(age_t* age, void *userData, unsigned int entity);
	void (*deleted)(age_t* age, void *userData, unsigned int entity);
} manager_t;
void manager_free(struct age_t *age, manager_t *manager);


typedef struct age_t {
	void *(*malloc)(size_t);
	void (*free)(void *);

	int initialized;
	int processing;

	// un ensemble pour reutiliser les ids libres
	sparseIntegerSet_t freeEntityIds;
	unsigned int nextEntityId;

	// entity data
	// first 'column' is bits of components defined
	// the rest are the components
	unsigned int dataWidth;
	unsigned int dataHeight;
	unsigned int dataHeightCapacity;
	void *data;

	unsigned int processingDataHeight;
	void **processingData;

	// buffer entity status notifications
	sparseIntegerSet_t added;
	sparseIntegerSet_t enabled;
	sparseIntegerSet_t disabled;
	sparseIntegerSet_t deleted;

	// all active entities (added and enabled)
	denseIntegerSet_t active;

	unsigned int num_components;
	component_t *components;

	unsigned int num_systems;
	system_t *systems;

	unsigned int num_managers;
	manager_t *managers;


	//Gestion des assets
		// un ensemble pour reutiliser les ids libres
	sparseIntegerSet_t freeAssetIds;
	unsigned int nextAssetId;
	unsigned int num_sprites;
	sprite_t* sprites[AGE_MAX_SPRITES];

} age_t;



// singleton pour le moteur de jeu
struct age_t;



//NEW: fonction qui ajoute un sprite au tableau et retourne son identifiant; retourne -1 en cas d erreur
// la structure sprite est copiee en profondeur ie avec copie du tableau .data et geree par le moteur
int age_add_sprite(struct age_t * ge, const sprite_t* sprite, int* index);
//NEW Fonction qui pointe vers le sprite d'index ind
int age_get_sprite(struct age_t * ge, int ind, sprite_t** result);
//NEW: fonction qui libere le sprite d indice ind en liberant aussi la memoire allouee pour la structure et pour .data 
int age_free_sprite(struct age_t * ge, int ind);

int age_initialize(struct age_t *);
int age_process(struct age_t *age, float delta);
int allocate_age(void *(*malloc)(size_t), void (*free)(void *), struct age_t ** r);
int age_free(struct age_t *age);

//creation d'un component
int age_createComponent(
	struct age_t *age,
	const char *name,
 	size_t size,
 	unsigned int flags,
	unsigned int * component_ptr
);

//creation d'un system

// system
int age_createSystem(
	struct age_t *age,
	const char *name,
	void (*starting)(struct age_t *, void *), //pointeur de fonction
	void (*process)(struct age_t *, void *, unsigned int, float), //pointeur de fonction
	void (*ending)(struct age_t *, void *), //pointeur de fonction
	void (*subscribed)(struct age_t *, void *, unsigned int), //pointeur de fonction
	void (*unsubscribed)(struct age_t *, void *, unsigned int), //pointeur de fonction
	void *userData,
	unsigned int flags,
	unsigned int * system_ptr
);

//demander a un system d'observer un composant 
int age_watch(struct age_t *age, unsigned int system, unsigned int component);

//demander a un system d'exclure un composant 
int age_exclude(struct age_t *age, unsigned int system, unsigned int component);



// --
//Les gestionnaire d'un composant
// manager
int age_createManager(
	struct age_t *diana,
	const char *name,
	void (*added)(struct age_t *, void *, unsigned int),
	void (*enabled)(struct age_t *, void *, unsigned int),
	void (*disabled)(struct age_t *, void *, unsigned int),
	void (*deleted)(struct age_t *, void *, unsigned int),
	void *userData,
	unsigned int flags,
	unsigned int * manager_ptr
);


//--
// Execution durant la boucle de jeu
// RUNTIME
int age_process(struct age_t *, float delta);

// demande de l execution d'un systeme particulier
int age_processSystem(struct age_t *, unsigned int system, float delta);


//Gestion des entites

// creation d'une entite
int age_spawn(struct age_t *age, unsigned int * entity_ptr);

// clonage d'une entite
int age_clone(struct age_t *age, unsigned int protoEntity, unsigned int * entity_ptr);

// envoyer un signal concernant une entite
int age_signal(struct age_t *age, unsigned int entity, unsigned int signal);

// gestion simple des compostants d une entite
int age_setComponent(struct age_t *age, unsigned int entity, unsigned int component, const void * data);
int age_getComponent(struct age_t *age, unsigned int entity, unsigned int component, void ** data_ptr);



int age_getComponentCount(struct age_t* age, unsigned int entity, unsigned int component, unsigned int * count_ptr);
int age_appendComponent(struct age_t* age, unsigned int entity, unsigned int component, const void * data);
int age_removeComponents(struct age_t* age, unsigned int entity, unsigned int component);


// fonction bas niveau pour la gestion des composants d une entite par index
int age_setComponentI(struct age_t *age, unsigned int entity, unsigned int component, unsigned int i, const void * data);
int age_getComponentI(struct age_t *age, unsigned int entity, unsigned int component, unsigned int i, void ** ptr);
int age_removeComponentI(struct age_t *age, unsigned int entity, unsigned int component, unsigned int i);



#endif
// code du moteur de jeu en entite systeme inspire du projet diana disponible sur https://github.com/discoloda/Diana/