#include <datamgt.h>
#include <string.h>
#include <assert.h>
#include <math.h>


int error(const char*);

list_t* list_new(age_t* m) 
{
  list_t* l;
  _malloc(m,sizeof(list_t),(void**) &l);
  l->num_items = 0;
  l->num_slots = 0;
  _malloc( m, sizeof(void*) * l->num_slots, (void**) &l->items  );
  return l;
}

static void list_reserve_more(age_t* e,list_t* l) {
  if (l->num_items > l->num_slots) {
    int  old_num_slots = l->num_slots;
    l->num_slots = ceil((l->num_slots + 1) * 1.5);
    //l->items = realloc(l->items, sizeof(void*) * l->num_slots);
    _realloc(e,l->items, sizeof(void*) * old_num_slots,sizeof(void*) * l->num_slots, (void**) &l->items);

  }
}

void list_push_back(age_t* e,list_t* l, void* item) {
  l->num_items++;
  list_reserve_more(e,l);
  l->items[l->num_items-1] = item;
}

static void list_reserve_less(age_t* e,list_t* l) {
  if ( l->num_slots > pow(l->num_items+1, 1.5)) {
    int  old_num_slots = l->num_slots;
    l->num_slots = floor((l->num_slots-1) * (1.0/1.5));
    //l->items = realloc(l->items, sizeof(void*) * l->num_slots);
    _realloc(e,l->items, sizeof(void*) * old_num_slots,sizeof(void*) * l->num_slots, (void**) &l->items);
  }
}

void* list_pop_back(age_t* e,list_t* l) {
  assert( l->num_items > 0 );
  
  void* item = l->items[ l->num_items-1 ];
  
  l->num_items--;
  list_reserve_less(e,l);
  
  return item;
}

void* list_pop_at(age_t* e,list_t* l, int index) {
  assert( l->num_items > 0 );
  
  void* item = l->items[index];
  
  memmove(&l->items[index], 
          &l->items[index+1], 
          sizeof(void*) * ((l->num_items-1) - index));
  
  l->num_items--;
  list_reserve_less(e,l);
  
  return item;
}

void* list_get(list_t* l, int index) {
  
  if (index < 0) { error("Index out of bounds"); return NULL; }
  if (index >= l->num_items) { error("Index out of bounds"); return NULL; }
  
  return l->items[index]; 
}


void list_set(list_t* l, int index, void* item) {

  if (index < 0) { error("Index out of bounds"); }
  if (index >= l->num_items) { error("Index out of bounds"); }

  l->items[index] = item;
}

bool list_is_empty(list_t* l) {
  return (l->num_items == 0);
}


void list_delete(age_t* e, list_t* l) {
  _free(e,l->items);
  _free(e,l);
}

void list_clear(age_t* e, list_t* l) {
  int old_num_slots = l->num_slots;
  l->num_items = 0;
  l->num_slots = 0;
  _realloc(e,l->items, sizeof(void*) * old_num_slots , sizeof(void*) * l->num_slots,(void**) &l->items);
}

void list_delete_with(age_t* e,list_t* l, void func(void*)) {
  
  while( l->num_items > 0) {
    void* item = list_pop_back(e,l);
    func(item);
  }
  
  _free(e,l->items);
  _free(e,l);
}

void list_clear_with(age_t* e,list_t* l, void func(void*)) {
  
  while( l->num_items > 0) {
    void* item = list_pop_back(e,l);
    func(item);
  }
  
}