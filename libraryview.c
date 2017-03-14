#include "libraryview.h"
#include <SDL/SDL_ttf.h>
#include <stdlib.h>
#include <stdio.h>

libraryview* libraryview_create(library *l) {
     libraryview *k = malloc(sizeof(*k));
     k->scroll = scrollbar_create();
     k->x = 0;
     k->y = 0;
     k->w = 0;
     k->h = 0;
     k->update = 0;
     k->l = l;
     return k;
}

void libraryview_free(libraryview *k) {
     scrollbar_free(k->scroll);
     free(k);
}

void libraryview_draw(libraryview *k, SDL_Surface *screen, int x, int y) {

     k->x = x;
     k->y = y;
     k->h = screen->h - y;
     k->w = 250 + scrollbar_width;

     scrollbar_draw(k->scroll, screen, k->x + k->w - scrollbar_width,
		    k->y);
     
     SDL_Rect um = {x: k->x, y: k->y,
		    w: k->w - scrollbar_width, h: k->h};

     // load the font
     TTF_Font *font=TTF_OpenFont("bitwise.ttf", 14);
     if(!font) {
	  printf("TTF_OpenFont: %s\n", TTF_GetError());
	  exit(1);
     }

     SDL_FillRect(screen, &um, SDL_MapRGB(screen->format, 255,100,100));

     SDL_Surface *text_surface;
     SDL_Color color={0,0,0};
     SDL_Color bgcolor={255,100,100};
     SDL_Color selcolor={255,255,100};
     SDL_Color playcolor={255,255,255};
     int padding = 1;
     int fontheight = TTF_FontHeight((font));
     SDL_Rect tum = {x: x + padding, y: y + padding,
		     w: k->w - scrollbar_width - padding * 2, h: fontheight};
     SDL_Rect sum = {x: 0, y: 0, w: tum.w, h: tum.h};


     for (int i=0; i<library_songcount(k->l); i++) {
	  if (tum.x >= x + um.w) break;
	  song *s = library_songat(k->l, i);
	  if (i == k->l->sl->selected)
	       text_surface=TTF_RenderText_Shaded(font,s->filename,color,
						  selcolor);
	  else if (i == k->l->sl->playing)
	       text_surface=TTF_RenderText_Shaded(font,s->filename,color,
						  playcolor);

	  else
	       text_surface=TTF_RenderText_Shaded(font,s->filename,color,
						  bgcolor);
	  if(!text_surface) {
	       //handle error here, perhaps print TTF_GetError at least
	       printf("error rendering text\n");
	       exit(EXIT_FAILURE);
	  } else {
	       SDL_BlitSurface(text_surface,&sum,screen,&tum);
	       //perhaps we can reuse it, but I assume not for simplicity.
	       SDL_FreeSurface(text_surface);
	  }
	  tum.y += fontheight + padding * 2;
     }

     TTF_CloseFont(font);
}
