
#include <stdlib.h>
#include <unistd.h>

#define ALIGNMENT 8
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~(ALIGNMENT-1))

#define ENTETE_SIZE (ALIGN(sizeof(bloc_entete))) 

void *myalloc0(size_t size);
void myfree0(void* ptr);
void bloc_info0(void* ptr);

void *myalloc1(size_t size);
void myfree1(void* ptr);
void bloc_info1(void* ptr);

void *myalloc2(size_t size);
void myfree2(void* ptr);
void bloc_info2(void* ptr);