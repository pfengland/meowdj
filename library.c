#include "library.h"
#include <stdlib.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>

library* library_create(void) {
     library *l = malloc(sizeof(*l));
     l->sl = songlist_create();
     return l;
}

void library_open(library *l) {
     DIR *dir;
     struct dirent *ent;
     if ((dir = opendir(library_dir)) != NULL) {
	  /* print all the files and directories within directory */
	  while ((ent = readdir(dir)) != NULL) {
	       printf("%s\n", ent->d_name);
	       if (strcmp(ent->d_name, ".") == 0) continue;
	       if (strcmp(ent->d_name, "..") == 0) continue;
	       songlist_addfile(l->sl, ent->d_name);
	  }
	  closedir(dir);
     } else {
	  /* could not open directory */
	  printf("error opening library directory\n");
	  exit(EXIT_FAILURE);
     }     
}

song* library_songat(library *l, int i) {
     return songlist_songat(l->sl, i);
}

int library_songcount(library *l) {
     return songlist_count(l->sl);
}


void library_free(library *l) {
     songlist_free(l->sl);
     free(l);
}
