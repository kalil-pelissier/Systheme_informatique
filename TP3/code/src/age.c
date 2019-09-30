#include <age.h>
#include <datamgt.h>
#include <string.h>
#include <util.h>
#include <limits.h>

static int _fixData(struct age_t *age);
static void _check(age_t *age, system_t *system, unsigned int entity);
static void _subscribe(age_t *age, struct _system *system, unsigned int entity) ;
static void _unsubscribe(age_t *age, struct _system *system, unsigned int entity);
static int _getComponentI(struct age_t *age, unsigned int entity, unsigned int component, unsigned int i, void ** ptr);
static int _setComponentI(struct age_t *age, unsigned int entity, unsigned int component, unsigned int i, const void * data);
static int _removeComponentI(struct age_t *age, unsigned int entity, unsigned int component, unsigned int i);


int allocate_age(void *(*malloc)(size_t), void (*free)(void *), age_t ** r) {
	*r = malloc(sizeof(**r));
	if(*r == NULL) {
		return AGE_ERROR_OUT_OF_MEMORY;
	}
	memset(*r, 0, sizeof(**r));
	(*r)->malloc = malloc;
	(*r)->free = free;
	return AGE_ERROR_NONE;
}


int age_free(age_t *age) {
	component_t *component;
	system_t *system;
	manager_t* manager;
	unsigned int i, j;

	int err = _fixData(age);
	
    if(err != AGE_ERROR_NONE) {
		return err;
	}

	for(i = 0; i < age->nextEntityId; i++) {
		for(j = 0; j < age->num_components; j++) {
			age_removeComponents(age, i, j);
		}
	}

	_free(age, age->data);
	sparseIntegerSet_free(age, &age->freeEntityIds);
	sparseIntegerSet_free(age, &age->added);
	sparseIntegerSet_free(age, &age->enabled);
	sparseIntegerSet_free(age, &age->disabled);
	sparseIntegerSet_free(age, &age->deleted);
	denseIntegerSet_free(age, &age->active);

	FOREACH_ARRAY(component, i, age->components, age->num_components) {
		component_free(age, component);
	}
	_free(age, age->components);

	FOREACH_ARRAY(system, i, age->systems, age->num_systems) {
		system_free(age, system);
	}
	_free(age, age->systems);

	FOREACH_ARRAY(manager, i, age->managers, age->num_managers) {
		manager_free(age, manager);
	}
	_free(age, age->managers);

	age->free(age);

	return AGE_ERROR_NONE;
}

// ============================================================================
// INITIALIZATION TIME
int age_initialize(age_t *age) {
	unsigned int extraBytes = (age->num_components + 7) >> 3, n;
	component_t * c;

	if(age->initialized) {
		return AGE_ERROR_INVALID_OPERATION;
	}

	FOREACH_ARRAY(c, n, age->components, age->num_components) {
		c->offset += extraBytes;
	}
	age->dataWidth += extraBytes;
	age->initialized = 1;
	return AGE_ERROR_NONE;
}

// ============================================================================
// RUNTIME

int age_process(struct age_t *age, float delta) {
	unsigned int entity, i, j;
	system_t *system;
	manager_t *manager;
	
	if(!age->initialized) {
		return AGE_ERROR_INVALID_OPERATION;
	}

	age->processing = 1;

	FOREACH_SPARSEINTSET(entity, i, &age->added) {
		FOREACH_ARRAY(manager, j, age->managers, age->num_managers) {
			if(manager->added != NULL) {
				manager->added(age, manager->userData, entity);
			}
		}
	}
	sparseIntegerSet_clear(age, &age->added);

	FOREACH_SPARSEINTSET(entity, i, &age->enabled) {
		FOREACH_ARRAY(system, j, age->systems, age->num_systems) {
			_check(age, system, entity);
		}
		FOREACH_ARRAY(manager, j, age->managers, age->num_managers) {
			if(manager->enabled != NULL) {
				manager->enabled(age, manager->userData, entity);
			}
		}
		denseIntegerSet_insert(age, &age->active, entity);
	}
	sparseIntegerSet_clear(age, &age->enabled);

	FOREACH_SPARSEINTSET(entity, i, &age->disabled) {
		FOREACH_ARRAY(system, j, age->systems, age->num_systems) {
			_unsubscribe(age, system, entity);
		}
		FOREACH_ARRAY(manager, j, age->managers, age->num_managers) {
			if(manager->disabled != NULL) {
				manager->disabled(age, manager->userData, entity);
			}
		}
		denseIntegerSet_delete(age, &age->active, entity);
	}
	sparseIntegerSet_clear(age, &age->disabled);

	FOREACH_SPARSEINTSET(entity, i, &age->deleted) {
		FOREACH_ARRAY(system, j, age->systems, age->num_systems) {
			_unsubscribe(age, system, entity);
		}

		FOREACH_ARRAY(manager, j, age->managers, age->num_managers) {
			if(manager->deleted != NULL) {
				manager->deleted(age, manager->userData, entity);
			}
		}
		for(j = 0; j < age->num_components; j++) {
			age_removeComponents(age, entity, j);
		}
		sparseIntegerSet_insert(age, &age->freeEntityIds, entity);
	}
	sparseIntegerSet_clear(age, &age->deleted);

	FOREACH_ARRAY(system, j, age->systems, age->num_systems) {
		if(system->flags & AGE_SYSTEM_PASSIVE_BIT) {
			continue;
		}

		if(system->starting != NULL) {
			system->starting(age, system->userData);
		}
		FOREACH_DENSEINTSET(entity, &system->entities) {
			system->process(age, system->userData, entity, delta);
		}
		if(system->ending != NULL) {
			system->ending(age, system->userData);
		}
	}

	age->processing = 0;

	return _fixData(age);

}

int _fixData(struct age_t *age) {
	// take care of spawns that happen during processing
	if(age->processingData != NULL) {
		unsigned int newDataHeight = age->dataHeight + age->processingDataHeight, i;

		if(newDataHeight >= age->dataHeightCapacity) {
			unsigned int newDataHeightCapacity = (newDataHeight + 1) * 1.5;
			int err = _realloc(age, age->data, age->dataWidth * age->dataHeightCapacity, age->dataWidth * newDataHeightCapacity, (void **)&age->data);
			if(err != AGE_ERROR_NONE) {
				return err;
			}
			age->dataHeightCapacity = newDataHeightCapacity;
		}

		for(i = 0; i < age->processingDataHeight; i++) {
			memcpy((unsigned char *)age->data + (age->dataWidth * (age->dataHeight + i)), age->processingData[i], age->dataWidth);
			age->free(age->processingData[i]);
		}
		age->free(age->processingData);

		age->dataHeight = newDataHeight;

		age->processingData = NULL;
		age->processingDataHeight = 0;
	}

	return AGE_ERROR_NONE;
}

// ============================================================================
// RUNTIME
static unsigned char *_getEntityData(age_t *age, unsigned int entity) {
	if(entity >= age->dataHeightCapacity) {
		return age->processingData[entity - age->dataHeightCapacity];
	}
	return (void *)((unsigned char *)age->data + (age->dataWidth * entity));
}

static void _subscribe(age_t *age, struct _system *system, unsigned int entity) {
	int included = denseIntegerSet_insert(age, &system->entities, entity);
	if(!included && system->subscribed != NULL) {
		system->subscribed(age, system->userData, entity);
	}
}

static void _unsubscribe(age_t *age, system_t *system, unsigned int entity) {
	int included = denseIntegerSet_delete(age, &system->entities, entity);
	if(included && system->unsubscribed != NULL) {
		system->unsubscribed(age, system->userData, entity);
	}
}

static void _check(age_t *age,  system_t *system, unsigned int entity) {
	unsigned char *entity_components = _getEntityData(age, entity);
	unsigned int component, i;
	int wanted = 1;

	FOREACH_SPARSEINTSET(component, i, &system->watch) {
		if(!(entity_components[component >> 3] & (1 << (component & 7)))) {
			wanted = 0;
			break;
		}
	}

	FOREACH_SPARSEINTSET(component, i, &system->exclude) {
		if((entity_components[component >> 3] & (1 << (component & 7)))) {
			wanted = 0;
			break;
		}
	}

	if(wanted) {
		_subscribe(age, system, entity);
	} else {
		_unsubscribe(age, system, entity);
	}
}


// ============================================================================
// ENTITIES

int age_spawn(struct age_t *age, unsigned int * entity_ptr) {
	unsigned int r;
	int err = AGE_ERROR_NONE;

	if(!age->initialized) {
		return AGE_ERROR_INVALID_OPERATION;
	}

	if(sparseIntegerSet_isEmpty(age, &age->freeEntityIds)) {
		r = age->nextEntityId++;
	} else {
		r = sparseIntegerSet_pop(age, &age->freeEntityIds);
	}

	age->dataHeight = age->dataHeight > (r + 1) ? age->dataHeight : (r + 1);

	if(age->dataHeight > age->dataHeightCapacity) {
		if(age->processing) {
			void *entityData;
		 
			err = _malloc(age, age->dataWidth, &entityData);
			if(err != AGE_ERROR_NONE) {
				return err;
			}

			if(entityData == NULL) {
				return 0;
			}

			err = _realloc(age, age->processingData, sizeof(*age->processingData) * age->processingDataHeight, sizeof(*age->processingData) * (age->processingDataHeight + 1), (void **)&age->processingData);
			if(err != AGE_ERROR_NONE) {
				return err;
			}

			age->processingData[age->processingDataHeight++] = entityData;
		} else {
			unsigned int newDataHeightCapacity = age->dataHeight * 1.5;
			err = _realloc(age, age->data, age->dataWidth * age->dataHeightCapacity, age->dataWidth * newDataHeightCapacity, (void **)&age->data);
			if(err != AGE_ERROR_NONE) {
				return err;
			}
			age->dataHeightCapacity = newDataHeightCapacity;
		}
	}

	*entity_ptr = r;

	return err;
}

int age_signal(struct age_t *age, unsigned int entity, unsigned int signal) {
	int err = AGE_ERROR_NONE;

	if(!age->initialized) {
		return AGE_ERROR_INVALID_OPERATION;
	}

	if((!age->processing && entity >= age->dataHeight) || (age->processing && entity >= age->dataHeightCapacity + age->processingDataHeight)) {
		return AGE_ERROR_INVALID_VALUE;
	}

	switch(signal) {
	case AGE_ENTITY_ADDED:
		sparseIntegerSet_insert(age, &age->added, entity);
		sparseIntegerSet_insert(age, &age->enabled, entity);
		sparseIntegerSet_delete(age, &age->disabled, entity);
		sparseIntegerSet_delete(age, &age->deleted, entity);
		break;
	case AGE_ENTITY_ENABLED:
		sparseIntegerSet_insert(age, &age->enabled, entity);
		sparseIntegerSet_delete(age, &age->disabled, entity);
		sparseIntegerSet_delete(age, &age->deleted, entity);
		break;
	case AGE_ENTITY_DISABLED:
		sparseIntegerSet_delete(age, &age->enabled, entity);
		sparseIntegerSet_insert(age, &age->disabled, entity);
		sparseIntegerSet_delete(age, &age->deleted, entity);
		break;
	case AGE_ENTITY_DELETED:
		sparseIntegerSet_delete(age, &age->added, entity);
		sparseIntegerSet_delete(age, &age->enabled, entity);
		sparseIntegerSet_insert(age, &age->disabled, entity);
		sparseIntegerSet_insert(age, &age->deleted, entity);
		break;
	default:
		err = AGE_ERROR_INVALID_VALUE;
	}

	return err;
}

int age_clone(struct age_t *age, unsigned int parentEntity, unsigned int * entity_ptr) {
	unsigned int newEntity, ci, cbi, cbn;
	unsigned char *parentEntityData;
	int err = AGE_ERROR_NONE;

	if(!age->initialized) {
		return AGE_ERROR_INVALID_OPERATION;
	}

	if((!age->processing && parentEntity >= age->dataHeight) || (age->processing && parentEntity >= age->dataHeightCapacity + age->processingDataHeight)) {
		return AGE_ERROR_INVALID_VALUE;
	}

	err = age_spawn(age, &newEntity);
	if(err != AGE_ERROR_NONE) {
		return err;
	}

	parentEntityData = _getEntityData(age, parentEntity);

	for(ci = 0; ci < age->num_components; ci++) {
		if(!bits_isSet(parentEntityData, ci)) {
			continue;
		}

		err = age_getComponentCount(age, parentEntity, ci, &cbn);
		if(err != AGE_ERROR_NONE) {
			return err;
		}

		for(cbi = 0; cbi < cbn; cbi++) {
			void *cd = NULL;
		  err = _getComponentI(age, parentEntity, ci, cbi, &cd);
			if(err != AGE_ERROR_NONE) {
				return err;
			}
			_setComponentI(age, newEntity, ci, cbi, cd);
		}
	}

	*entity_ptr = newEntity;

	return err;
}


// ============================================================================
// COMPONENTS

int age_removeComponents(age_t *age, unsigned int entity, unsigned int component) {
	unsigned char *entityData;
	struct component_t *c;
	unsigned int i;

	if(!age->initialized) {
		return AGE_ERROR_INVALID_OPERATION;
	}

	if((!age->processing && entity >= age->dataHeight) || (age->processing && entity >= age->dataHeightCapacity + age->processingDataHeight)) {
		return AGE_ERROR_INVALID_VALUE;
	}

	if(component >= age->num_components) {
		return AGE_ERROR_INVALID_VALUE;
	}

	entityData = _getEntityData(age, entity);
	c = age->components + component;

	if(c->flags & AGE_COMPONENT_MULTIPLE_BIT) {
		struct componentBag_t *bag = (componentBag_t *)(entityData + c->offset);
		if(bag->count) {
			for(i = 0; i < bag->count; i++) {
				sparseIntegerSet_insert(age, &c->freeDataIndexes, bag->indexes[i]);
			}
			bag->count = 0;
			age->free(bag->indexes);
			bag->indexes = NULL;
		}
		return AGE_ERROR_NONE;
	} else {
		return _removeComponentI(age, entity, component, 0);
	}
}


// single
int age_setComponent(struct age_t *age, unsigned int entity, unsigned int component, const void * data) {
	if(!age->initialized) {
		return AGE_ERROR_INVALID_OPERATION;
	}

	if((!age->processing && entity >= age->dataHeight) || (age->processing && entity >= age->dataHeightCapacity + age->processingDataHeight)) {
		return AGE_ERROR_INVALID_VALUE;
	}

	if(component >= age->num_components) {
		return AGE_ERROR_INVALID_VALUE;
	}

	return _setComponentI(age, entity, component, 0, data);
}

int age_getComponent(struct age_t *age, unsigned int entity, unsigned int component, void ** ptr) {
	if(!age->initialized) {
		return AGE_ERROR_INVALID_OPERATION;
	}

	if((!age->processing && entity >= age->dataHeight) || (age->processing && entity >= age->dataHeightCapacity + age->processingDataHeight)) {
		return AGE_ERROR_INVALID_VALUE;
	}

	if(component >= age->num_components) {
		return AGE_ERROR_INVALID_VALUE;
	}

	return _getComponentI(age, entity, component, 0, ptr);
}

int age_removeComponent(struct age_t *age, unsigned int entity, unsigned int component) {
	if(!age->initialized) {
		return AGE_ERROR_INVALID_OPERATION;
	}

	if((!age->processing && entity >= age->dataHeight) || (age->processing && entity >= age->dataHeightCapacity + age->processingDataHeight)) {
		return AGE_ERROR_INVALID_VALUE;
	}

	if(component >= age->num_components) {
		return AGE_ERROR_INVALID_VALUE;
	}

	return _removeComponentI(age, entity, component, 0);
}

// multiple
int age_getComponentCount(struct age_t *age, unsigned int entity, unsigned int component, unsigned int * count_ptr) {
	unsigned char *entityData;
	struct component_t *c;

	if(!age->initialized) {
		return AGE_ERROR_INVALID_OPERATION;
	}

	if((!age->processing && entity >= age->dataHeight) || (age->processing && entity >= age->dataHeightCapacity + age->processingDataHeight)) {
		return AGE_ERROR_INVALID_VALUE;
	}

	if(component >= age->num_components) {
		return AGE_ERROR_INVALID_VALUE;
	}

	entityData = _getEntityData(age, entity);
	c = age->components + component;

	if(c->flags & AGE_COMPONENT_MULTIPLE_BIT) {
		componentBag_t *bag = (componentBag_t *)(entityData + c->offset);
		*count_ptr = bag->count;
	} else {
		*count_ptr = bits_isSet(entityData, component);
	}

	return AGE_ERROR_NONE;
}

int age_appendComponent(struct age_t *age, unsigned int entity, unsigned int component, const void * data) {
	struct component_t *c;

	if(!age->initialized) {
		return AGE_ERROR_INVALID_OPERATION;
	}

	if((!age->processing && entity >= age->dataHeight) || (age->processing && entity >= age->dataHeightCapacity + age->processingDataHeight)) {
		return AGE_ERROR_INVALID_VALUE;
	}

	if(component >= age->num_components) {
		return AGE_ERROR_INVALID_VALUE;
	}

	c = age->components + component;

	if(c->flags & AGE_COMPONENT_MULTIPLE_BIT) {
		unsigned int cc = 0;
		age_getComponentCount(age, entity, component, &cc);
		return _setComponentI(age, entity, component, cc, data);
	} else {
		return _setComponentI(age, entity, component, 0, data);
	}
}


// low level
int age_setComponentI(struct age_t *age, unsigned int entity, unsigned int component, unsigned int i, const void * data) {
	if(!age->initialized) {
		return AGE_ERROR_INVALID_OPERATION;
	}

	if((!age->processing && entity >= age->dataHeight) || (age->processing && entity >= age->dataHeightCapacity + age->processingDataHeight)) {
		return AGE_ERROR_INVALID_VALUE;
	}

	if(component >= age->num_components) {
		return AGE_ERROR_INVALID_VALUE;
	}

	return _setComponentI(age, entity, component, i, data);
}

int age_getComponentI(struct age_t *age, unsigned int entity, unsigned int component, unsigned int i, void ** ptr) {
	if(!age->initialized) {
		return AGE_ERROR_INVALID_OPERATION;
	}

	if((!age->processing && entity >= age->dataHeight) || (age->processing && entity >= age->dataHeightCapacity + age->processingDataHeight)) {
		return AGE_ERROR_INVALID_VALUE;
	}

	if(component >= age->num_components) {
		return AGE_ERROR_INVALID_VALUE;
	}

	return _getComponentI(age, entity, component, i, ptr);
}

int age_removeComponentI(struct age_t *age, unsigned int entity, unsigned int component, unsigned int i) {
	if(!age->initialized) {
		return AGE_ERROR_INVALID_OPERATION;
	}

	if((!age->processing && entity >= age->dataHeight) || (age->processing && entity >= age->dataHeightCapacity + age->processingDataHeight)) {
		return AGE_ERROR_INVALID_VALUE;
	}

	if(component >= age->num_components) {
		return AGE_ERROR_INVALID_VALUE;
	}

	return _removeComponentI(age, entity, component, i);
}

static int _getAComponentIndex(struct age_t *age, struct component_t *c, unsigned int * index) {
	if(sparseIntegerSet_isEmpty(age, &c->freeDataIndexes)) {
		if(c->flags & AGE_COMPONENT_LIMITED_BIT) {
			return AGE_ERROR_FULL_COMPONENT;
		}

		*index = c->nextDataIndex++;

		return _realloc(age, c->data, sizeof(void *) * *index, sizeof(void *) * c->nextDataIndex, (void **)&c->data) ||
		       _malloc(age, c->size, (void **)&c->data[*index]);
	} else {
		*index = sparseIntegerSet_pop(age, &c->freeDataIndexes);
	}

	return AGE_ERROR_NONE;
}

static int _setComponentI(struct age_t *age, unsigned int entity, unsigned int component, unsigned int i, const void * data) {
	unsigned char *entityData = _getEntityData(age, entity);
	struct component_t *c = age->components + component;
	int defined = bits_set(entityData, component);
	void *componentData = NULL;
	unsigned int err = AGE_ERROR_NONE;



	if(c->flags & AGE_COMPONENT_MULTIPLE_BIT) {
		componentBag_t *bag = (componentBag_t *)(entityData + c->offset);
		unsigned int index;

		if(i >= bag->count) {
			err = _getAComponentIndex(age, c, &index);
			if(err != AGE_ERROR_NONE) {
				return err;
			}

			err = _realloc(age, bag->indexes, sizeof(unsigned int) * bag->count, sizeof(unsigned int) * (bag->count + 1), (void **)&bag->indexes);
			if(err != AGE_ERROR_NONE) {
				return err;
			}
			bag->indexes[i = bag->count++] = index;
		}

		componentData = (void *)((unsigned char *)c->data[bag->indexes[i]]);
	} else if(c->flags & AGE_COMPONENT_INDEXED_BIT) {
		unsigned int *index = (unsigned int *)(entityData + c->offset);

		if(!defined) {
			err = _getAComponentIndex(age, c, index);
			if(err != AGE_ERROR_NONE) {
				return err;
			}
		}

		componentData = (void *)((unsigned char *)c->data[*index]);
	} else {
		componentData = (void *)(entityData + c->offset);
	}

	if(data != NULL) {
		memcpy(componentData, data, c->size);
	}

	return err;
}

static int _getComponentI(struct age_t *age, unsigned int entity, unsigned int component, unsigned int i, void ** ptr) {
	unsigned char *entityData = _getEntityData(age, entity);
	struct component_t *c = age->components + component;
	void *componentData = NULL;
	int err = AGE_ERROR_NONE;


	if(!bits_isSet(entityData, component)) {
		return AGE_ERROR_INVALID_VALUE;
	}


	if(c->flags & AGE_COMPONENT_MULTIPLE_BIT) {
		componentBag_t *bag = (componentBag_t *)(entityData + c->offset);
		if(i >= bag->count) {
			return AGE_ERROR_INVALID_VALUE;
		}
		componentData = (void *)((unsigned char *)c->data[bag->indexes[i]]);
	} else if(c->flags & AGE_COMPONENT_INDEXED_BIT) {
		unsigned int *index = (unsigned int *)(entityData + c->offset);
		if(*index == UINT_MAX) {
			return err;
		}
		componentData = (void *)((unsigned char *)c->data[*index]);
	} else {
		componentData = (void *)(entityData + c->offset);
	}


	*ptr = componentData;

	return err;
}

static int _removeComponentI(struct age_t *age, unsigned int entity, unsigned int component, unsigned int i) {
	unsigned char *entityData = _getEntityData(age, entity);
	struct component_t *c = age->components + component;
	int err = AGE_ERROR_NONE;

	if(!bits_clear(entityData, component)) {
		return err;
	}

	if(c->flags & AGE_COMPONENT_MULTIPLE_BIT) {
		componentBag_t *bag = (componentBag_t *)(entityData + c->offset);
		if(i >= bag->count) {
			return err;
		}
		sparseIntegerSet_insert(age, &c->freeDataIndexes, bag->indexes[i]);
		memcpy(bag->indexes + i, bag->indexes + i + 1, bag->count - i * sizeof(unsigned int));
		err = _realloc(age, bag->indexes, sizeof(unsigned int) * bag->count, sizeof(unsigned int) * (bag->count - 1), (void **)&bag->indexes);
		if(err != AGE_ERROR_NONE) {
			return err;
		}
		bag->count--;
		return err;
	}

	if(c->flags & AGE_COMPONENT_INDEXED_BIT) {
		unsigned int *index = (unsigned int *)(entityData + c->offset);
		sparseIntegerSet_insert(age, &c->freeDataIndexes, *index);
		*index = 0;
	}

	return err;
}
