#include "song.h"
#include "library.h"
#include <stdlib.h>
#include <string.h>
#include <sndfile.h>

song* song_create(const char *file) {
     song *s = malloc(sizeof(*s));
     s->filename = malloc(sizeof(char) * strlen(file) + 1);
     strcpy(s->filename, file);
     return s;
}

void song_free(song *s) {
     free(s->filename);
     free(s);
}

void song_load(song *s, audio_buffer *buffer) {

     // free any previously loaded data
     audio_buffer_free_samples(buffer);

     // load the new song
     char path[1000];
     snprintf(path, 1000, "%s/%s", library_dir, s->filename);
     printf("opening %s\n", path);
     SF_INFO info;
     info.format = 0;
     SNDFILE *sf = sf_open(path, SFM_READ, &info);

     // prepare the buffer for reading
     buffer->count = info.frames;
     buffer->samples = malloc(sizeof(jack_default_audio_sample_t) *
			      buffer->count);
     buffer->start = 0;
     buffer->end = buffer->count - 1;
     buffer->playing = 0;

     float *samples = malloc(sizeof(float) * buffer->count * info.channels);
     int rc = sf_readf_float(sf, samples, info.frames);
     printf("%d samples read\n", rc);
     // copy first channel to buffer
     for (int i=0; i<buffer->count; i++) {
	  buffer->samples[i] = samples[i*info.channels];
     }
     free(samples);
     sf_close(sf);
}
