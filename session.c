#include "session.h"
#include <stdlib.h>

session* session_create(void) {
     session *s = malloc(sizeof(*s));
     s->playing = 0;
     s->l = library_create();
     s->buffer = audio_buffer_create();
     return s;
}

void session_init(session *s) {
     library_open(s->l);
}

void session_free(session *s) {
     audio_buffer_free(s->buffer);
     library_free(s->l);
     free(s);
}
