#include <sprite.h>
#include <age.h>
#include <string.h>
#include <util.h>
#include <datamgt.h>


int age_sprite_clone( struct age_t* age, const sprite_t* src, sprite_t** clone){
    sprite_t* ptr = NULL;
    _malloc(age,sizeof(sprite_t),(void**)&ptr);
    memcpy(ptr,src,sizeof(sprite_t));
    int n = ptr->height*ptr->width*sizeof(char);
    _malloc(age,n,(void**)&ptr->data);
    memcpy(ptr->data,src->data,n);
    *clone = ptr;
    return OK;
};

int age_sprite_copy( struct age_t* age, const sprite_t* src, sprite_t* cpy){
    memcpy(cpy,src,sizeof(sprite_t));
    int n = cpy->height*cpy->width*sizeof(char);
    memcpy(cpy->data,src->data,n);
    return OK;
};


int age_sprite_free(struct age_t* age, sprite_t* sprite){
    _free(age,sprite);
    return OK;
};

int age_add_sprite(struct age_t * ge, const sprite_t* sprite, int* index){
 if(ge->num_sprites < AGE_MAX_SPRITES)
 {
    int id = -1;
    //avoir un index pour la sprite
    if(sparseIntegerSet_isEmpty( ge,&ge->freeAssetIds))
    {
        // nous devons creer un nouvel index
         id = ge->nextAssetId++;
    }else
    { 
        /*Ici je peux recuperer un ancien index*/
        id = sparseIntegerSet_pop(ge,&ge->freeAssetIds);
    }
    sprite_t* localsprite = NULL;
    age_sprite_clone(ge,sprite,&localsprite);
    ge->sprites[id] = localsprite;
    ge->num_sprites++;
    *index = id;
    return OK;
 }
 else{
     return KO;
 }
};
//NEW Fonction qui pointe vers le sprite d'index ind
int age_get_sprite(struct age_t * ge, int ind, sprite_t** result)
{
    if(ind < ge->num_sprites){
        result = &ge->sprites[ind];
        return OK;
    }
    return KO;
};

//NEW: fonction qui libere le sprite d indice ind en liberant aussi la memoire allouee pour la structure et pour .data 
int age_free_sprite(struct age_t * ge, int ind)
{
    sprite_t* sp = ge->sprites[ind];
    _free(ge,sp->data);
    _free(ge,sp);
    sparseIntegerSet_insert(ge,&ge->freeAssetIds,ind);
    ge->num_sprites--;
    return OK;
};