#ifndef H_SONG
#define H_SONG

#include "audio_buffer.h"

typedef struct song_s song;
struct song_s {
     char *filename;
};

song* song_create(const char *file);
void song_free(song *s);
void song_load(song *s, audio_buffer *buffer);

#endif
