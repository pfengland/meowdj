#ifndef H_LIBRARY
#define H_LIBRARY

#include "songlist.h"

#define library_dir "/home/forrest/meowdjtracks"

typedef struct library_s library;
struct library_s {
     songlist *sl;
};

library* library_create(void);
void library_open(library *l);
song* library_songat(library *l, int i);
int library_songcount(library *l);
void library_free(library *l);

#endif
