#include <age.h>
#include <datamgt.h>
#include <string.h>

void component_free(age_t *age, component_t *component) {
    //
    unsigned int i = 0;
	_free(age, (void *)component->name);
	for(i = 0; i < component->nextDataIndex; i++) {
		_free(age, component->data[i]);
	}
	_free(age, component->data);
	sparseIntegerSet_free(age, &component->freeDataIndexes);
	memset(component, 0, sizeof(*component));
};

int age_createComponent(
	age_t *age,
	const char *name,
 	size_t size,
 	unsigned int flags,
	unsigned int * component_ptr
) {
	component_t c;
	int err = AGE_ERROR_NONE;

	if(age->initialized) {
		return AGE_ERROR_INVALID_OPERATION;
	}

	memset(&c, 0, sizeof(c));
	err = _strdup(age, name, (char **)&c.name);
	if(err != AGE_ERROR_NONE) {
		return err;
	}
	c.size = size;
	c.flags = flags;
	c.offset = age->dataWidth;

	if(flags & AGE_COMPONENT_MULTIPLE_BIT) {
		size = sizeof(componentBag_t);
	} else if(flags & AGE_COMPONENT_INDEXED_BIT) {
		size = sizeof(unsigned int);
	}

	age->dataWidth += size;

	if(flags & AGE_COMPONENT_LIMITED_BIT) {
		unsigned int count = (flags >> 3);
		err = _malloc(age, c.size * count, (void **)&c.data);
		if(err != AGE_ERROR_NONE) {
			_free(age, (void *)c.name);
			return err;
		}
		while(count) {
			sparseIntegerSet_insert(age, &c.freeDataIndexes, count--);
		}
	}

	err = _realloc(age, age->components, sizeof(*age->components) * age->num_components, sizeof(*age->components) * (age->num_components + 1), (void **)&age->components);
	if(err != AGE_ERROR_NONE) {
		_free(age, (void *)c.name);
		return err;
	}
	age->components[age->num_components++] = c;

	*component_ptr = age->num_components - 1;

	return err;
}
