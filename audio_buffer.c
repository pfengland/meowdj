/* audio buffer
   pause tape */

#include <stdlib.h>
#include <stdio.h>
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

    buffer->speedchange_callback = NULL;
    buffer->callback_arg = NULL;

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

int audio_buffer_interpolate(audio_buffer *buffer, float *sample) {

     int skip = (int)buffer->speed;
     double mu = buffer->speed - skip;

     double s0, s1, s2, s3;
     if ((int)buffer->playing >= 0) {
	  s0 = 0.0;
	  s1 = buffer->samples[(int)buffer->playing];
	  s2 = buffer->samples[(int)buffer->playing + skip];
	  s3 = buffer->samples[(int)buffer->playing + skip * 2];
     } else if ((int)buffer->playing >= buffer->count - 1) {
	  s0 = buffer->samples[(int)buffer->playing - skip];
	  s1 = buffer->samples[(int)buffer->playing];
	  s2 = 0;
	  s3 = 0;
     } else {
	  s0 = buffer->samples[(int)buffer->playing - skip];
	  s1 = buffer->samples[(int)buffer->playing];
	  s2 = buffer->samples[(int)buffer->playing + skip];
	  s3 = buffer->samples[(int)buffer->playing + skip * 2];
     }

     double mu2 = mu*mu;
     double a0 = s3 - s2 - s0 + s1;
     double a1 = s0 - s1 - a0;
     double a2 = s2 - s0;
     double a3 = s1;
     *sample = a0*mu*mu2+a1*mu2+a2*mu+a3;

	  //     *sample = buffer->samples[(int)buffer->playing];
     
     buffer->playing += buffer->speed;

     if (buffer->playing >= buffer->count) {
	  buffer->playing = 0.0;
	  return 0;
     } else {
	  return 1;
     }
}

void audio_buffer_setspeed(audio_buffer *b, double val) {
     b->speed = val;
     printf("buffer speed set to %f\n", val);
     if (b->speedchange_callback)
	  b->speedchange_callback(b->callback_arg, val);
}

void audio_buffer_speedchange_callback(audio_buffer *b,
				       audio_buffer_callback cbk,
				       void *arg) {
     b->speedchange_callback = cbk;
     b->callback_arg = arg;
}
