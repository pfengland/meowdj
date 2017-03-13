#ifndef H_WINDOW
#define H_WINDOW

#include <SDL/SDL.h>
#include "waveview.h"
#include "libraryview.h"
#include "session.h"

static const int window_frameRate = 30;
// key repeat rate
static const int window_initrepeat = 300;
static const int window_repeat = 100;

typedef struct window_s window;
struct window_s {
     SDL_Surface *screen;
     waveview *wave;
     libraryview *library;
     session *s;
     int update;
     int quit;
     int framestart;
     int downpressed;
     int uppressed;
     SDL_TimerID timer;
};

window* window_create(session *s);
void window_free(window *w);
void window_mainLoop(window *w);
void window_handleEvents(window *w);
void window_repeatEvents(window *w);
void window_update(window *w);

#endif
