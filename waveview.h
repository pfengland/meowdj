#ifndef H_WAVEVIEW
#define H_WAVEVIEW

#include "audio_buffer.h"
#include "slider.h"
#include <SDL/SDL.h>

typedef struct waveview_s waveview;
struct waveview_s {
     audio_buffer *buffer;
     int x, y, h, w;
     int update;
     slider *pitch;
};

waveview* waveview_create(audio_buffer *b);
void waveview_free(waveview *k);
void waveview_draw(waveview *k, SDL_Surface *screen, int x, int y, int active);
void waveview_mousedown(waveview *w, int x, int y);
void waveview_mouseup(waveview *w, int x, int y);
void waveview_mousemove(waveview *w, int x, int y);
int waveview_needupdate(waveview *w);
void waveview_speedchanged(void *arg, double val);
     
#endif
