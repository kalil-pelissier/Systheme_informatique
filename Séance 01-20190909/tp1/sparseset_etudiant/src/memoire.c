#include <age.h>
#include <data.h>
#include <datamgt.h>
#include <string.h>

int _malloc(age_t *age, size_t size, void ** r) {
	*r = age->malloc(size);
	if(*r == NULL) {
		return AGE_ERROR_OUT_OF_MEMORY;
	}
	memset(*r, 0, size);
	return AGE_ERROR_NONE;
}

int _free(age_t *age, void *ptr) {
	if(ptr != NULL) {
		age->free(ptr);
	}
	return AGE_ERROR_NONE;
}

int _strdup(age_t *age, const char *s, char ** r) {
	unsigned int l;
	if(s == NULL) {
		return AGE_ERROR_NONE;
	}
	l = strlen(s);
	*r = age->malloc(l + 1);
	if(*r == NULL) {
		return AGE_ERROR_OUT_OF_MEMORY;
	}
	memcpy(*r, s, l + 1);
	return AGE_ERROR_NONE;
}

int _realloc(age_t *age, void *ptr, size_t oldSize, size_t newSize, void ** r) {
	if(oldSize == newSize) {
		*r = ptr;
		return AGE_ERROR_NONE;
	}
	if(newSize == 0) {
		_free(age, ptr);
		return AGE_ERROR_NONE;
	}
	*r = age->malloc(newSize);
	if(*r == NULL) {
		return AGE_ERROR_OUT_OF_MEMORY;
	}
	if(oldSize < newSize) {
		memset((unsigned char *)(*r) + oldSize, 0, newSize - oldSize);
	}
	if(ptr != NULL) {
		memcpy(*r, ptr, oldSize < newSize ? oldSize : newSize);
		_free(age, ptr);
	}
	return AGE_ERROR_NONE;
}
