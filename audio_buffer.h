#ifndef H_AUDIO_BUFFER
#define H_AUDIO_BUFFER

/* audio buffer ui
   pause tape 
   an audio buffer
   which can be played or captured into */

#include <jack/jack.h>

// our buffer to hold the captured samples
typedef struct audio_buffer_s audio_buffer;
struct audio_buffer_s {
     // audio sample buffer
     jack_default_audio_sample_t *samples;
     // sample count
     int count;
     // trim start sample
     int start;
     // trim end sample
     int end;
     // playing sample
     double playing;
     double speed;
};

// allocate and initialize the buffer
audio_buffer* audio_buffer_create(void);
audio_buffer* audio_buffer_init(audio_buffer *buffer);
void audio_buffer_free(audio_buffer *buffer);
void audio_buffer_free_samples(audio_buffer *buffer);
float audio_buffer_interpolate(audio_buffer *buffer);

#endif
