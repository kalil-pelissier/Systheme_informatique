#include <age.h>

#include <datamgt.h>
#include <string.h>


// ============================================================================
// system
int age_createSystem(
	age_t *age,
	const char *name,
	void (*starting)(age_t *, void *),
	void (*process)(age_t *, void *, unsigned int, float),
	void (*ending)(age_t *, void *),
	void (*subscribed)(age_t *, void *, unsigned int),
	void (*unsubscribed)(age_t *, void *, unsigned int),
	void *userData,
	unsigned int flags,
	unsigned int * system_ptr
) {
	struct _system s;
	int err = AGE_ERROR_NONE;

	if(age->initialized) {
		return AGE_ERROR_INVALID_OPERATION;
	}

	memset(&s, 0, sizeof(s));
	err = _strdup(age, name, (char **)&s.name);
	if(err != AGE_ERROR_NONE) {
		return err;
	}
	s.starting = starting;
	s.process = process;
	s.ending = ending;
	s.subscribed = subscribed;
	s.unsubscribed = unsubscribed;
	s.userData = userData;
	s.flags = flags;

	err = _realloc(age, age->systems, sizeof(*age->systems) * age->num_systems, sizeof(*age->systems) * (age->num_systems + 1), (void **)&age->systems);
	if(err != AGE_ERROR_NONE) {
		_free(age, (void *)s.name);
		return err;
	}
	age->systems[age->num_systems++] = s;

	*system_ptr = age->num_systems - 1;

	return err;
}

int age_watch(age_t *age, unsigned int system, unsigned int component) {
	if(age->initialized) {
		return AGE_ERROR_INVALID_OPERATION;
	}

	if(system >= age->num_systems) {
		return AGE_ERROR_INVALID_VALUE;
	}

	if(component >= age->num_components) {
		return AGE_ERROR_INVALID_VALUE;
	}

	sparseIntegerSet_insert(age, &age->systems[system].watch, component);

	return AGE_ERROR_NONE;
}

int age_exclude(age_t *age, unsigned int system, unsigned int component) {
	if(age->initialized) {
		return AGE_ERROR_INVALID_OPERATION;
	}

	if(system >= age->num_systems) {
		return AGE_ERROR_INVALID_VALUE;
	}

	if(component >= age->num_components) {
		return AGE_ERROR_INVALID_VALUE;
	}

	sparseIntegerSet_insert(age, &age->systems[system].exclude, component);

	return AGE_ERROR_NONE;
}

void system_free(age_t *age, system_t *system) {
	_free(age, (void *)system->name);
	sparseIntegerSet_free(age, &system->watch);
	sparseIntegerSet_free(age, &system->exclude);
	denseIntegerSet_free(age, &system->entities);
	memset(system, 0, sizeof(*system));
}