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
     l->w->wave->buffer = l->s->buffer;
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

     dj *l = arg;

     printf("cc: %d, %d\n", num, val);

     if (num == 118 && val == 127) {
	  audioclient_start_capture(l->audio);
     } else if (num == 116 && val == 127) {
	  audioclient_stop_capture(l->audio);
	  /*	  SF_INFO info = {frames: l->buffer->count,
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
	  int wc = sf_writef_double(f, (double*)l->buffer->samples,
				    l->buffer->count);
	  printf("%d samples written\n", wc);
	  if (wc != l->buffer->count) printf("sample write count mismatch\n");
	  sf_write_sync(f);
	  sf_close(f); */
     }
}

void dj_output(void *arg, jack_nframes_t nframes,
		   jack_default_audio_sample_t *in,
		   jack_default_audio_sample_t *out) {

     dj *l = arg;

     // calculate each sample for the current buffer
    for (int i=0; i<nframes; i++) {

        // capture - soft playthru
        if (l->audio->capturing) {

	    out[i] = in[i];

	} else if (l->s->playing) {
	     // play the requested samples
	     // if we're off the end stop playing
	     if (i >= l->s->buffer->count) {
		  l->s->playing = 0;
		  out[i] = 0;
	     } else {
		  out[i] = audio_buffer_interpolate(l->s->buffer);
	     }
	} else {
	    out[i] = 0;
	}
    }
}

void dj_process(dj *l) {
     audioclient_process_capture(l->audio, l->s->buffer);
     if (l->audio->capturing) l->w->update = 1;
     window_mainLoop(l->w);
}

void dj_free(dj *l) {
     window_free(l->w);
     session_free(l->s);
     midiclient_free(l->midi);
     audioclient_free(l->audio);
}

