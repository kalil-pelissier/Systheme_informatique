#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <age.h>
#include <datamgt.h>
#include <util.h>
#include <string.h>
struct age_t* ge;

void startTest()
{
    allocate_age(malloc, free, &ge);
    age_initialize(ge);
}

void endTest(){
    age_free(ge);
}

int testSparseIntSet()
{
    startTest();
    
    sparseIntegerSet_t _set;
    memset(&_set, 0, sizeof(_set)); // tres important en C
    // test de int sparseIntegerSet_insert(struct age_t *age, sparseIntegerSet_t *is, unsigned int i);
    int tab[] = {0,1,1,2,3,5,8,13,21,34,55,89,144};
    int tab_len = 13;
    int i;
    int v;
    int* ptr;
    FOREACH_ARRAY(ptr,i,tab,tab_len)
    {
        sparseIntegerSet_insert( ge, &_set , tab[i]);
    }
    FOREACH_SPARSEINTSET( v, i , &_set)
    {
        printf("valeur [%d]th est: %d \n",i,v);
    }
    
    endTest();
    
    return OK;
}



int main() {
    printf("starting...\n");
    fflush(stdout);
    testSparseIntSet();
    printf("end...\n");
    fflush(stdout);    
}