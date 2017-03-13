#ifndef H_LIBRARYVIEW
#define H_LIBRARYVIEW

#include "library.h"
#include "scrollbar.h"
#include <SDL/SDL.h>

typedef struct libraryview_s libraryview;
struct libraryview_s {
     library *l;
     int x, y, h, w;
     int update;
     scrollbar *scroll;
};

libraryview* libraryview_create(library *l);
void libraryview_free(libraryview *k);
void libraryview_draw(libraryview *k, SDL_Surface *screen, int x, int y);

#endif
