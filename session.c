#include "session.h"
#include <stdlib.h>

session* session_create(void) {
     session *s = malloc(sizeof(*s));
     s->l = library_create();
     s->deck1 = deck_create();
     s->deck1->active = 1;
     s->deck2 = deck_create();
     s->autoplay = 1;
     return s;
}

void session_init(session *s) {
     library_open(s->l);
}

void session_free(session *s) {
     deck_free(s->deck1);
     deck_free(s->deck2);
     library_free(s->l);
     free(s);
}
