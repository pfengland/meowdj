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

waveview* waveview_create(void);
void waveview_free(waveview *k);
void waveview_draw(waveview *k, SDL_Surface *screen, int x, int y);

#endif
