#ifndef H_DECK
#define H_DECK

#include "audio_buffer.h"

typedef struct deck_s deck;
struct deck_s {
     audio_buffer *buffer;
     int playing;
     int active;
};

deck* deck_create(void);
void deck_free(deck *d);
void deck_setspeed(deck *d, double speed);
int deck_output(deck *d, float *sample);

#endif
