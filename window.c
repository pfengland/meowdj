#include "window.h"
#include <stdlib.h>
#include <X11/Xlib.h>
#include <SDL/SDL_ttf.h>

window* window_create(session *s) {

     window *w = malloc(sizeof(*w));

     w->update = 1;
     w->quit = 0;
     w->framestart = 0;
     w->s = s;

     XInitThreads();
     SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER);
     if (TTF_Init() == -1) {
	  printf("TTF_Init: %s\n", TTF_GetError());
	  exit(1);
     }

     // get the screen
     //    int opts = SDL_SWSURFACE|SDL_NOFRAME;
     int opts = SDL_SWSURFACE|SDL_RESIZABLE;
     w->screen = SDL_SetVideoMode(640, 480, 32, opts);

     SDL_WM_SetCaption("MEOW", NULL);

     w->wave1 = waveview_create(s->deck1->buffer);
     w->wave2 = waveview_create(s->deck2->buffer);
     w->library = libraryview_create(w->s->l);

     return w;
}

void window_free(window *w) {
     waveview_free(w->wave1);
     waveview_free(w->wave2);
     libraryview_free(w->library);
     TTF_Quit();
     SDL_Quit();
     free(w);
}

void window_mainLoop(window *w) {

     window_handleEvents(w);
     window_update(w);
}

void window_update(window *w) {

     // limit framerate
     int elapsed = SDL_GetTicks() - w->framestart;
     int fps = 20;
     int frametime = 1000 / fps;
     if (elapsed < frametime) {
	  return;
     }

     if (w->update || waveview_needupdate(w->wave1) ||
	 waveview_needupdate(w->wave2) || w->library->update ||
	 w->s->deck1->playing || w->s->deck2->playing) {

	  w->update = 0;

	  SDL_FillRect(w->screen, NULL,
		       SDL_MapRGB(w->screen->format, 0,100,100));

	  libraryview_draw(w->library, w->screen, 0, 0);
	  waveview_draw(w->wave1, w->screen, w->library->w, 0,
			w->s->deck1->active);
	  waveview_draw(w->wave2, w->screen, w->library->w, w->wave1->h,
			w->s->deck2->active);

	  SDL_Flip(w->screen);

	  w->framestart = SDL_GetTicks();
     } 
}

Uint32 window_timeout(Uint32 interval, void *win) {
     window *w = win;
     if (w->uppressed) {
	  songlist_selectdec(w->s->l->sl);
	  w->update = 1;
     } else if (w->downpressed) {
	  songlist_selectinc(w->s->l->sl);
	  w->update = 1;
     }
     
     return window_repeat;
}

void window_handleEvents(window *w) {

     SDL_Event event;
     song *s;

     while (SDL_PollEvent(&event)) {

	  if (event.type == SDL_KEYDOWN) {
	       switch (event.key.keysym.sym) {
		case SDLK_UP:
		     songlist_selectdec(w->s->l->sl);
		     w->update = 1;
		     w->uppressed = 1;
		     w->timer = SDL_AddTimer(window_initrepeat,
					     window_timeout,
					     w);
		     break;
	       case SDLK_DOWN:
		    songlist_selectinc(w->s->l->sl);
		    w->update = 1;
		    w->downpressed = 1;
		    w->timer = SDL_AddTimer(window_initrepeat,
					    window_timeout, w);
		    break;
	       case SDLK_RETURN:
		    if (w->s->deck1->active) {
			 // stop playing
			 w->s->deck1->playing = 0;
			 // load the song
			 s = songlist_getselected(w->s->l->sl);
			 w->s->l->sl->playing = w->s->l->sl->selected;
			 printf("loading song %s on deck 1\n", s->filename);
			 song_load(s, w->s->deck1->buffer);
			 w->update = 1;
		    } else if (w->s->deck2->active) {
			 // stop playing
			 w->s->deck2->playing = 0;
			 // load the song
			 s = songlist_getselected(w->s->l->sl);
			 w->s->l->sl->playing = w->s->l->sl->selected;
			 printf("loading song %s on deck 2\n", s->filename);
			 song_load(s, w->s->deck2->buffer);
			 w->update = 1;
		    }
		    break;
	       case SDLK_SPACE:
		    if (w->s->deck1->active) {
			 // toggle playing
			 w->s->deck1->playing = ! w->s->deck1->playing;
			 if (w->s->deck1->playing) printf("playing deck 1\n");
			 else printf("stopping deck 1\n");
		    } else if (w->s->deck2->active) {
			 // toggle playing
			 w->s->deck2->playing = ! w->s->deck2->playing;
			 if (w->s->deck2->playing) printf("playing deck 2\n");
			 else printf("stopping deck 2\n");
		    }
	       default:
		    break;
	       }
	  } else if (event.type == SDL_KEYUP) {
	       switch (event.key.keysym.sym) {
		case SDLK_UP:
		     w->uppressed = 0;
		     SDL_RemoveTimer(w->timer);
		     break;
	       case SDLK_DOWN:
		    w->downpressed = 0;
		    SDL_RemoveTimer(w->timer);
		    break;
	       default:
		    break;
	       }
	  } else if (event.type == SDL_QUIT) {
	       w->quit = 1;
	  } else if (event.type == SDL_VIDEORESIZE) {
	       int opts = SDL_SWSURFACE|SDL_RESIZABLE;
	       w->screen = SDL_SetVideoMode(event.resize.w, event.resize.h,
					    32, opts);
	       w->update = 1;
	  } else if (event.type == SDL_MOUSEBUTTONDOWN) {
	       int x = event.button.x;
	       int y = event.button.y;
	       if (x >= w->wave1->x && x < w->wave1->x + w->wave1->w &&
		   y >= w->wave1->y && y < w->wave1->y + w->wave1->h) {

		    waveview_mousedown(w->wave1, x, y);
	       }
	  } else if (event.type == SDL_MOUSEBUTTONUP) {
	       int x = event.button.x;
	       int y = event.button.y;

	       if (x >= w->wave1->x && x < w->wave1->x + w->wave1->w &&
		   y >= w->wave1->y && y < w->wave1->y + w->wave1->h) {

		    waveview_mouseup(w->wave1, x, y);
	       }

	  } else if (event.type == SDL_MOUSEMOTION) {
	       int x = event.motion.x;
	       int y = event.motion.y;
	       if (x >= w->wave1->x && x < w->wave1->x + w->wave1->w &&
		   y >= w->wave1->y && y < w->wave1->y + w->wave1->h) {

		    waveview_mousemove(w->wave1, x, y);
	       }
	  }
     }
}
