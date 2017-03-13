#ifndef H_SESSION
#define H_SESSION

#include "library.h"
#include "audio_buffer.h"

typedef struct session_s session;
struct session_s {
     library *l;
     audio_buffer *buffer;
     int playing;
};

session* session_create(void);
void session_init(session *s);
void session_free(session *s);

#endif
