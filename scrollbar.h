#ifndef H_SCROLLBAR
#define H_SCROLLBAR

#include <SDL/SDL.h>

#define scrollbar_width 15

typedef enum {
     scrollbar_vertical,
     scrollbar_horizontal
} scrollbar_orientation;

typedef struct scrollbar_s scrollbar;
struct scrollbar_s {
     int range;
     int scrollrange;
     int start;
     int x, y, h, w;
     int update;
     int orientation;
};

scrollbar* scrollbar_create(void);
void scrollbar_free(scrollbar *sb);
void scrollbar_draw(scrollbar *sb, SDL_Surface *screen, int x, int y);

#endif
