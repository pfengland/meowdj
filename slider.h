#ifndef H_SLIDER
#define H_SLIDER

#include <SDL/SDL.h>

typedef struct slider_s slider;
struct slider_s {
     int x, y, h, w;
     int hx, hy, hh, hw;
     int sliding;
     int pixrange;
     int pixmin;
     int pixmax;
     int mdy;
     double min;
     double max;
     double value;
     int update;
};

slider* slider_create(void);
void slider_free(slider *s);
void slider_draw(slider *s, SDL_Surface *screen, int x, int y, int h);
void slider_mousedown(slider *s, int x, int y);
void slider_mouseup(slider *s, int x, int y);
void slider_mousemove(slider *s, int x, int y);

#endif
