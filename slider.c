#include "slider.h"
#include <stdlib.h>

slider* slider_create(void) {
     slider *s = malloc(sizeof(*s));
     s->x = 0;
     s->y = 0;
     s->h = 0;
     s->w = 0;

     s->hx = 0;
     s->hy = 0;
     s->hh = 0;
     s->hw = 0;

     s->sliding = 0;
     s->pixrange = 0;
     s->pixmin = 0;
     s->pixmax = 0;
     
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
     s->pixrange = ratio * (um.h - handleh / 2);
     s->pixmin = um.y;
     s->pixmax = um.y + um.h;

     um.x = s->x + pady;
     um.y = s->y + s->h - s->pixrange - handleh;
     um.h = handleh;
     um.w = s->w - pady * 2;

     s->hx = um.x;
     s->hy = um.y;
     s->hh = um.h;
     s->hw = um.w;
     
     SDL_FillRect(screen, &um, SDL_MapRGB(screen->format, 0,0,0));
}

void slider_mousedown(slider *s, int x, int y) {
     if (x >= s->hx && x < s->hx + s->hw &&
	 y >= s->hy && y < s->hy + s->hh) {
	  printf("sliding started\n");
	  s->mdy = y;
	  s->sliding = 1;
     }
}

void slider_mouseup(slider *s, int x, int y) {
     s->sliding = 0;
     printf("sliding stopped\n");
}

void slider_mousemove(slider *s, int x, int y) {

     /*     if (s->sliding) {

	  double dy = y - s->mdy;

	  double ratio = (double)dy / (double)(s->pixrange);
	  
	  s->value += ratio * (s->max - s->min);
	  if (s->value < s->min) s->value = s->min;
	  if (s->value > s->max) s->value = s->max;
	  s->update = 1;
	  printf("sliding to ratio %f, %f\n", ratio, s->value);
	  printf("y = %d, pixmin = %d, pixmax = %d, pixrange = %d\n",
		 y, s->pixmin,
		 s->pixmax, s->pixrange);
		 } */
}
