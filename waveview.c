#include "waveview.h"
#include <SDL/SDL_ttf.h>
#include <stdlib.h>

waveview* waveview_create(audio_buffer *b) {
     waveview *k = malloc(sizeof(*k));
     k->pitch = slider_create();
     k->x = 0;
     k->y = 0;
     k->w = 0;
     k->h = 0;
     k->update = 0;
     k->buffer = b;
     audio_buffer_speedchange_callback(b, &waveview_speedchanged, k);

     return k;
}

void waveview_speedchanged(void *arg, double val) {
     waveview *w = arg;
     w->pitch->value = val;
     w->pitch->update = 1;
}

void waveview_free(waveview *k) {
     slider_free(k->pitch);
     free(k);
}

int waveview_needupdate(waveview *w) {
     return w->update || w->pitch->update;
}

void waveview_draw(waveview *k, SDL_Surface *screen, int x, int y, int active) {

     k->x = x;
     k->y = y;
     k->h = 200;
     k->w = screen->w - x;

     slider_draw(k->pitch, screen, k->x, k->y, k->h);
     
     SDL_Rect um = {x: k->x + k->pitch->w, y: k->y,
		    w: k->w - k->pitch->w, h: k->h};

     if (active) {
	  SDL_FillRect(screen, &um, SDL_MapRGB(screen->format, 100,0,0));
     } else {
	  SDL_FillRect(screen, &um, SDL_MapRGB(screen->format, 0,0,0));
     }

     if (!k->buffer->samples) return;

    // half of vertical size to calculate where to draw the sample
    int halfy = k->h/2;
    int samplewidth = k->buffer->count / k->w - k->pitch->w;

    // draw a recangle for each horizontal pixel
    for (x=0; x < k->w - k->pitch->w; x++) {

	// find out which samples
        // need to be drawn in this location
	int samplestart = samplewidth * x;
	int sampleend = (x + 1) * samplewidth - 1;

	// go through our samples and find the max and min

	double maxsample = -1, minsample=1;
        int start_marker = 0;
        int end_marker = 0;
        int play_marker = 0;
	for (int si=samplestart; si<sampleend; si++) {

            if (k->buffer->samples[si] > maxsample) {
		maxsample = k->buffer->samples[si];
	    }
	    if (k->buffer->samples[si] < minsample) {
		minsample = k->buffer->samples[si];
	    }

            if (si == k->buffer->start) {
                start_marker = 1;
            } else if (si == k->buffer->end) {
                end_marker = 1;
            } else if (si == (int)k->buffer->playing) {
                play_marker = 1;
            }
	}

	// got the max and min. draw it
	int y = halfy + minsample * halfy;
	int height = (maxsample - minsample) * halfy;
	if (!height) height = 1;
	SDL_Rect rum = {x: k->x + k->pitch->w + x,
			y: k->y + y, w: 1, h: height};
	SDL_FillRect(screen, &rum, SDL_MapRGB(screen->format, 100,0,100));

        SDL_Rect mr = {x: k->x + k->pitch->w + x, y: k->y, w: 1, h: k->h};
        if (start_marker) {
            SDL_FillRect(screen, &mr, SDL_MapRGB(screen->format, 100,100,0));
        } else if (end_marker) {
            SDL_FillRect(screen, &mr, SDL_MapRGB(screen->format, 100,100,100));
        } else if (play_marker) {
            SDL_FillRect(screen, &mr, SDL_MapRGB(screen->format, 0, 200, 0));
        }
    }
}

void waveview_mousedown(waveview *w, int x, int y) {
     if (x >= w->pitch->x && x < w->pitch->x + w->pitch->w &&
	 y >= w->pitch->y && x < w->pitch->y + w->pitch->h) {

	  slider_mousedown(w->pitch, x, y);
     }
}

void waveview_mouseup(waveview *w, int x, int y) {
          if (x >= w->pitch->x && x < w->pitch->x + w->pitch->w &&
	 y >= w->pitch->y && x < w->pitch->y + w->pitch->h) {

	  slider_mouseup(w->pitch, x, y);
     }
}

void waveview_mousemove(waveview *w, int x, int y) {
          if (x >= w->pitch->x && x < w->pitch->x + w->pitch->w &&
	 y >= w->pitch->y && x < w->pitch->y + w->pitch->h) {

	  slider_mousemove(w->pitch, x, y);
     }
}

