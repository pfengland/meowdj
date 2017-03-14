#include "dj.h"
#include <stdlib.h>
#include <stdio.h>
#include <sndfile.h>

dj* dj_create(void) {
     dj *l = malloc(sizeof(*l));
     l->audio = audioclient_create();
     l->midi = midiclient_create();
     l->s = session_create();
     l->w = window_create(l->s);
     l->autoplay = 0;
     return l;
}

void dj_init(dj *l) {
     audioclient_init(l->audio);
     midiclient_start(l->midi);
     audioclient_set_process_callback(l->audio, dj_output, l);
     midiclient_set_cc_callback(l->midi, dj_cc, l);
     session_init(l->s);
}

void dj_stop(dj *l) {
     audioclient_stop(l->audio);
}

void dj_cc(void *arg, unsigned int num, unsigned int val) {

     dj *d = arg;

     printf("cc: %d, %d\n", num, val);

     if (num == 118 && val == 127) {
	  audioclient_start_capture(d->audio);
     } else if (num == 116 && val == 127) {
	  audioclient_stop_capture(d->audio);
     } else if (num == 32) {
	  double ratio = (double)val / 127.0;
	  if (d->s->deck1->active)
	       deck_setspeed(d->s->deck1, ratio * 2.0);
	  if (d->s->deck2->active)
	       deck_setspeed(d->s->deck2, ratio * 2.0);
	  d->w->update = 1;
     } else if (num == 24) {
	  d->s->deck1->active = 1;
	  d->s->deck2->active = 0;
	  d->w->update = 1;
     } else if (num == 25) {
	  d->s->deck1->active = 0;
	  d->s->deck2->active = 1;
	  d->w->update = 1;
     }
	  /*	  SF_INFO info = {frames: d->buffer->count,
			 samplerate: 44100,
			 channels: 1,
			 format: SF_FORMAT_WAV|SF_FORMAT_FLOAT,
			 sections: 0,
			 seekable: 0};
	  SNDFILE *f = sf_open("meowdjrecording.wav", SFM_WRITE, &info);
	  if (!f) {
	       printf("opening sound file for writing failed");
	       return;
	  }
	  int wc = sf_writef_double(f, (double*)d->buffer->samples,
				    d->buffer->count);
	  printf("%d samples written\n", wc);
	  if (wc != d->buffer->count) printf("sample write count mismatch\n");
	  sf_write_sync(f);
	  sf_close(f);
     } */
}

void dj_output(void *arg, jack_nframes_t nframes,
	       jack_default_audio_sample_t *in,
	       jack_default_audio_sample_t *out1,
	       jack_default_audio_sample_t *out2) {

     dj *d = arg;
     float deck1s = 0.0, deck2s = 0.0;

     // calculate each sample for the current buffer
    for (int i=0; i<nframes; i++) {

        // capture - soft playthru
	 //        if (d->audio->capturing) {
	 //	    captures = in[i];
	 //	}
	if (d->s->deck1->playing) {
	     if (!deck_output(d->s->deck1, &deck1s)) {

		  d->s->deck1->playing = 0;
		  
		  if (d->s->autoplay &&
		      d->s->l->sl->playing <
		      d->s->l->sl->songs->count - 1) {

		       d->s->l->sl->playing++;
		       d->autoplay = 1;
		  }
	     }
	}
	if (d->s->deck2->playing) {
	     if (!deck_output(d->s->deck2, &deck2s)) {
		  d->s->deck2->playing = 0;
	     }
	}
	out1[i] = deck1s;
	out2[i] = deck2s;
    }
}

void dj_process(dj *l) {

     audioclient_process_capture(l->audio, l->s->deck1->buffer);

     if (l->audio->capturing) l->w->update = 1;
     window_mainLoop(l->w);
     if (l->autoplay) {
	  l->autoplay = 0;
	  song_load(songlist_songat(l->s->l->sl, l->s->l->sl->playing),
		    l->s->deck1->buffer);
	  l->s->deck1->playing = 1;
	  l->w->update = 1;
     }
}

void dj_free(dj *l) {
     window_free(l->w);
     session_free(l->s);
     midiclient_free(l->midi);
     audioclient_free(l->audio);
}

