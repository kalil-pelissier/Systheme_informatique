#include<util.h>
#include<datamgt.h>

#include <string.h>
#include <limits.h>

int _malloc(struct age_t *a, size_t size, void ** r);
int _realloc(struct age_t *a, void *ptr, size_t oldSize, size_t newSize, void ** r);
int _free(struct age_t *a, void *ptr);


unsigned int denseIntegerSet_insert(struct age_t *age, denseIntegerSet_t *is, unsigned int i) {
	if(i >= is->capacity) {
		unsigned int newCapacity = (i + 1) * 1.5;
		_realloc(age, is->bytes, (is->capacity + 7) >> 3, (newCapacity + 7) >> 3, (void **)&is->bytes);
		is->capacity = newCapacity;
	}
	return bits_set(is->bytes, i);
}

unsigned int denseIntegerSet_delete(struct age_t *age, denseIntegerSet_t *is, unsigned int i) {
	if(i < is->capacity) {
		return bits_clear(is->bytes, i);
	}
	return 0;
}


void denseIntegerSet_clear(age_t *age, denseIntegerSet_t *is) {
	memset(is->bytes, 0, (is->capacity + 7) >> 3);
}

int denseIntegerSet_isEmpty(age_t *age, denseIntegerSet_t *is) {
	unsigned int n = (is->capacity + 7) >> 3, i = 0;
	for(; i < n; i++) {
		if(is->bytes[i]) {
			return 0;
		}
	}
	return 1;
}

void denseIntegerSet_free(age_t *age, denseIntegerSet_t *is) {
	_free(age, is->bytes);
	memset(is, 0, sizeof(*is));
}
