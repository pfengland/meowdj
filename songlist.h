#ifndef H_SONGLIST
#define H_SONGLIST

#include "dynamicarray.h"
#include "song.h"

typedef struct songlist_s songlist;
struct songlist_s {
     dynamicarray *songs;
     int selected;
     int playing;
};

songlist* songlist_create(void);
song* songlist_songat(songlist *sl, int i);
int songlist_count(songlist *sl);
void songlist_addfile(songlist *sl, const char *f);
void songlist_free(songlist *sl);
void songlist_selectdec(songlist *sl);
void songlist_selectinc(songlist *sl);
song* songlist_getselected(songlist *sl);

#endif
