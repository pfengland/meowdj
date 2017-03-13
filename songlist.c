#include "songlist.h"
#include "song.h"
#include <stdlib.h>

songlist* songlist_create(void) {
     songlist *sl = malloc(sizeof(*sl));
     sl->selected = 0;
     sl->songs = dynamicarray_create();
     sl->songs->items = malloc(sizeof(song*) * sl->songs->size);
     for (int i=0; i<sl->songs->size; i++) {
	  ((song**)sl->songs->items)[i] = NULL;
     }
     return sl;
}

void songlist_addfile(songlist *sl, const char *f) {
     int newcount = sl->songs->count + 1;
     int i;
     song *s = song_create(f);
     if (sl->songs->size < newcount) {
	  int newsize = sl->songs->size * 2;
	  sl->songs->items = realloc(sl->songs->items, sizeof(*s) * newsize);
	  for (i=sl->songs->size; i<newsize; i++) {
	       ((song**)sl->songs->items)[i] = NULL;
	  }
	  sl->songs->size = newsize;
     }
     ((song**)sl->songs->items)[sl->songs->count] = s;
     sl->songs->count = newcount;
}

void songlist_free(songlist *sl) {
     int i;
     for (i=0; i<sl->songs->count; i++) {
	  song_free(((song**)sl->songs->items)[i]);
	  ((song**)sl->songs->items)[i] = NULL;
     }
     free(sl->songs->items);
     dynamicarray_free(sl->songs);
     free(sl);
}

song* songlist_songat(songlist *sl, int i) {
     if (sl->songs->count <= i) return NULL;
     return((song**)sl->songs->items)[i];
}

song* songlist_getselected(songlist *sl) {
     return songlist_songat(sl, sl->selected);
}

int songlist_count(songlist *sl) {
     return sl->songs->count;
}

void songlist_selectdec(songlist *sl) {
     sl->selected--;
     if (sl->selected < 0) sl->selected = 0;
}

void songlist_selectinc(songlist *sl) {
     sl->selected++;
     if (sl->selected >= sl->songs->count)
	  sl->selected = sl->songs->count - 1;
}

