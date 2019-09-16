#include <age.h>
#include <datamgt.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <util.h>
#include <string.h>


#include <ncurses.h>
#include <sprite.h>
#include <unistd.h>

//TODO: Objectif de la seance, charger une sprite a partir d'un fichier texte. 
int age_asset_load_sprite(age_t* ge, const char* path, sprite_t* dest);


WINDOW *create_newwin(int height, int width, int starty, int startx);
void destroy_win(WINDOW *local_win);
struct age_t* ge;
static WINDOW* _win;
int displaySprite(WINDOW *my_win, int x , int y,sprite_t * sp);
void loadSprites();

const int numsprites = 4;
sprite_t sprites[4];


struct position {
	float x, y;
};
static unsigned int positionComponent;


struct renderer {
	int sprite;
};
static unsigned int rendererComponent;

static void renderSystem_process(struct age_t *age, void *user_data, unsigned int entity, float delta) {
	struct position *position;
	struct renderer *renderer;
	age_getComponent(age, entity, positionComponent, (void **)&position);
	age_getComponent(age, entity, rendererComponent, (void **)&renderer);
    displaySprite(_win, position->x , position->y,age->sprites[ renderer->sprite] );
}

void startTest()
{
    allocate_age(malloc, free, &ge);
}

void endTest(){
    age_free(ge);
}

void renderSystem_start(age_t* ge, void * data){
    //printf("Render Start");
    wclear(_win);
}
void renderSystem_end(age_t* ge, void * data){
    //printf("Render Start");
    box(_win, 0 , 0);
    wmove(_win,10,10);
    wrefresh(_win);
}


void initRendering(){
    initscr();			/* Start curses mode 		  */
    raw();
    _win = create_newwin(50, 100, 0, 0);
    nodelay(_win, TRUE);
    keypad(_win, TRUE);
    noecho();
}

void endRendering()
{
    endwin();
}

int testSprite()
{
    startTest();
    loadSprites();
    
    struct renderer r = {0};    
    unsigned int movementSystem, renderSystem, e;
    age_createComponent(ge, "position", sizeof(struct position), AGE_COMPONENT_FLAG_INLINE, &positionComponent);
	age_createComponent(ge, "renderer", sizeof(struct renderer), AGE_COMPONENT_FLAG_INLINE, &rendererComponent);

    initRendering();
    age_createSystem(ge, "render", renderSystem_start, renderSystem_process, renderSystem_end, NULL, NULL, NULL, AGE_SYSTEM_FLAG_NORMAL, &renderSystem);
	age_watch(ge, renderSystem, positionComponent);
	age_watch(ge, renderSystem, rendererComponent);
    
    age_initialize(ge);
    
    //load ressources 
    int index;
    age_add_sprite(ge,&sprites[0],&index);
    age_add_sprite(ge,&sprites[1],&index);
    age_add_sprite(ge,&sprites[2],&index);
    age_add_sprite(ge,&sprites[3],&index);
    


    
    age_spawn(ge, &e);
    struct position p = {1,1};
    age_setComponent(ge, e, positionComponent, &p);
	age_setComponent(ge, e, rendererComponent, &r);
	age_signal(ge, e, AGE_ENTITY_ADDED);

    int i = 0;
    int input = 0;
    double const fps = 1.0/30.0 ;
    double const ufps = fps * 1000000;
    input = wgetch(_win);
    long frame = 0;
    while(input != 32) //SPACE code 32
    {
		// 30 fps
        //
        age_process(ge, fps);
        wmove(_win,1,1);
        wprintw(_win,"<space> pour quitter");
        wrefresh(_win);
		usleep(ufps);
        input = wgetch(_win);
	}
    endRendering();
    endTest();    
    return OK;
}


int displaySprite(WINDOW *my_win, int x , int y,sprite_t* sp)
{
    int i,j;
    for(i = 0 ; i < sp->width; i++)
        for(j = 0 ; j < sp->height; j++)
        {
            wmove(my_win,y+j,x+i );
            waddch(my_win,sp->data[i+j*sp->width]);
        }
    return 0;
}






WINDOW *create_newwin(int height, int width, int starty, int startx)
{	WINDOW *local_win;

	local_win = newwin(height, width, starty, startx);
	box(local_win, 0 , 0);		/* 0, 0 gives default characters 
					 * for the vertical and horizontal
					 * lines			*/
	wrefresh(local_win);		/* Show that box 		*/

	return local_win;
}

void destroy_win(WINDOW *local_win)
{	
	/* box(local_win, ' ', ' '); : This won't produce the desired
	 * result of erasing the window. It will leave it's four corners 
	 * and so an ugly remnant of window. 
	 */
	wborder(local_win, ' ', ' ', ' ',' ',' ',' ',' ',' ');
	/* The parameters taken are 
	 * 1. win: the window on which to operate
	 * 2. ls: character to be used for the left side of the window 
	 * 3. rs: character to be used for the right side of the window 
	 * 4. ts: character to be used for the top side of the window 
	 * 5. bs: character to be used for the bottom side of the window 
	 * 6. tl: character to be used for the top left corner of the window 
	 * 7. tr: character to be used for the top right corner of the window 
	 * 8. bl: character to be used for the bottom left corner of the window 
	 * 9. br: character to be used for the bottom right corner of the window
	 */
	wrefresh(local_win);
	delwin(local_win);
}

void loadSprites()
{   
    age_asset_load_sprite(ge,"./assets/image.1.txt",&sprites[0]);
}


int main() {
    printf("starting...\n");
    fflush(stdout);
    testSprite();
    printf("end...\n");
    fflush(stdout);
}