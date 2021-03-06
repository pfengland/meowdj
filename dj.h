#ifndef H_DJ
#define H_DJ

#include "midiclient.h"
#include "audioclient.h"
#include "window.h"
#include "session.h"
#include <jack/jack.h>

typedef struct dj_s dj;
struct dj_s {
     audioclient *audio;
     midiclient *midi;
     window *w;
     session *s;
     int autoplay;
};

dj* dj_create(void);
void dj_init(dj *l);
void dj_stop(dj *l);
void dj_output(void *arg, jack_nframes_t nframes,
	       jack_default_audio_sample_t *in,
	       jack_default_audio_sample_t *out1,
	       jack_default_audio_sample_t *out2);
void dj_process(dj *l);
void dj_free(dj *l);
void dj_cc(void *arg, unsigned int num, unsigned int val);

#endif

