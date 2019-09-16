#include <age.h>
#include <data.h>
#include <datamgt.h>
#include <string.h>
#include <util.h>
#include <limits.h>

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


int age_free(age_t *age) 
{
	age->free(age);
	return AGE_ERROR_NONE;
}

// ============================================================================
// INITIALIZATION TIME
int age_initialize(age_t *age) {
	if(age->initialized) {
		return AGE_ERROR_INVALID_OPERATION;
	}
	age->initialized = 1;
	return AGE_ERROR_NONE;
}