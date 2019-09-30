#include <age.h>
#include <datamgt.h>
#include <string.h>


void manager_free(age_t *age, manager_t *manager) {
	_free(age, (void *)manager->name);
	memset(manager, 0, sizeof(*manager));
}