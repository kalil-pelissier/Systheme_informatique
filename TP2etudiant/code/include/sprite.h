#ifndef __SPRITE__H
#define __SPRITE__H


struct age_t* age;

typedef struct sprite_t {
unsigned short height;
unsigned short width;
char* data;
} sprite_t ;

int age_sprite_clone(struct age_t* age, const sprite_t* src, sprite_t** clone);
int age_sprite_free(struct age_t* age, sprite_t* sprite);

#endif
