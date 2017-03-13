/* audio buffer
   pause tape */

#include <stdlib.h>
#include <assert.h>
#include "audio_buffer.h"

audio_buffer* audio_buffer_create(void) {
    audio_buffer *buffer = malloc(sizeof(*buffer));
    return audio_buffer_init(buffer);
}

audio_buffer* audio_buffer_init(audio_buffer *buffer) {

    assert(buffer);

    buffer->samples = NULL;
    buffer->count = 0;
    buffer->start = 0;
    buffer->end = 0;
    buffer->playing = 0;
    buffer->speed = 1;

    return buffer;
}

void audio_buffer_free(audio_buffer *buffer) {

    assert(buffer);
    audio_buffer_free_samples(buffer);
    free(buffer);
}

void audio_buffer_free_samples(audio_buffer *buffer) {

    assert(buffer);
    if (buffer->samples) {
        free(buffer->samples);
        buffer->samples = NULL;
    }
}

float audio_buffer_interpolate(audio_buffer *buffer) {
     float s = buffer->samples[(int)buffer->playing];
     buffer->playing += buffer->speed;
     return s;
}
