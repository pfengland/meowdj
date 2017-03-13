#include "slider.h"
#include <stdlib.h>

slider* slider_create(void) {
     slider *s = malloc(sizeof(*s));
     s->x = 0;
     s->y = 0;
     s->h = 0;
     s->w = 0;
     s->update = 0;
     s->min = 0.0;
     s->max = 2.0;
     s->value = 1.0;
     return s;
}

void slider_free(slider *s) {
     free(s);
}

void slider_draw(slider *s, SDL_Surface *screen, int x, int y, int h) {
     s->x = x;
     s->y = y;
     s->w = 30;
     s->h = h;
     SDL_Rect um = {x: s->x, y: s->y, w: s->w, h: s->h};
     SDL_FillRect(screen, &um, SDL_MapRGB(screen->format, 100,100,100));

     // draw slot
     int slotw = 2;
     int pady = 5;
     um.x = s->x + s->w/2 - slotw/2;
     um.y = s->y + pady;
     um.w = slotw;
     um.h = s->h - pady*2;
     SDL_FillRect(screen, &um, SDL_MapRGB(screen->format, 50,50,50));

     // draw handle
     double ratio = (s->value - s->min) / (s->max - s->min);
     int handleh = 20;
     int pos = ratio * (um.h - handleh / 2);
     um.x = s->x + pady;
     um.y = s->y + pos;
     um.h = handleh;
     um.w = s->w - pady * 2;
     SDL_FillRect(screen, &um, SDL_MapRGB(screen->format, 0,0,0));
}
