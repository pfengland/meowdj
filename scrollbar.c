#include "scrollbar.h"
#include <stdlib.h>

scrollbar* scrollbar_create(void) {
     scrollbar *sb = malloc(sizeof(*sb));
     sb->range = 0;
     sb->scrollrange = 0;
     sb->start = 0;
     sb->x = 0;
     sb->y = 0;
     sb->h = 0;
     sb->w = scrollbar_width;
     return sb;
}

void scrollbar_free(scrollbar *sb) {
     free(sb);
}

void scrollbar_draw(scrollbar *sb, SDL_Surface *screen, int x, int y) {

     sb->x = x;
     sb->y = y;
     sb->h = screen->h - y;
     
     SDL_Rect um = {x: sb->x, y: sb->y,
		    w: sb->w, h: sb->h};

     SDL_FillRect(screen, &um, SDL_MapRGB(screen->format, 100,100,255));

     // scroll down control
     um.h = scrollbar_width;
     SDL_FillRect(screen, &um, SDL_MapRGB(screen->format, 100,255,255));
     // scroll up control
     um.y = sb->y + sb->h - scrollbar_width;
     SDL_FillRect(screen, &um, SDL_MapRGB(screen->format, 100,255,255));
     // scroll handle
     um.y = sb->y + scrollbar_width;
     SDL_FillRect(screen, &um, SDL_MapRGB(screen->format, 255,100,255));
}
