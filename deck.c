#include "deck.h"
#include <stdlib.h>

deck* deck_create(void) {
     deck *d = malloc(sizeof(*d));
     d->playing = 0;
     d->active = 0;
     d->buffer = audio_buffer_create();
     return d;
}

void deck_free(deck *d) {
     audio_buffer_free(d->buffer);
     free(d);
}

void deck_setspeed(deck *d, double speed) {
     audio_buffer_setspeed(d->buffer, speed);
}

int deck_output(deck *d, float *sample) {
     return audio_buffer_interpolate(d->buffer, sample);
}
