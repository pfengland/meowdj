#ifndef H_SESSION
#define H_SESSION

#include "library.h"
#include "audio_buffer.h"
#include "deck.h"

typedef struct session_s session;
struct session_s {
     library *l;
     deck *deck1;
     deck *deck2;
     int autoplay;
};

session* session_create(void);
void session_init(session *s);
void session_free(session *s);

#endif
