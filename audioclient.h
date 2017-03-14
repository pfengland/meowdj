#ifndef H_AUDIOCLIENT
#define H_AUDIOCLIENT

#include <jack/jack.h>
#include <jack/ringbuffer.h>
#include "audio_buffer.h"

#define audioclient_ringbuffer_size 16384

typedef void (*process_callback_t)(void *arg, jack_nframes_t nframes,
				   jack_default_audio_sample_t *in,
				   jack_default_audio_sample_t *out1,
				   jack_default_audio_sample_t *out2);


typedef struct audioclient_s audioclient;
struct audioclient_s {
     jack_client_t *client;
     jack_port_t *input_port;
     jack_port_t *output_port1;
     jack_port_t *output_port2;
     jack_ringbuffer_t *ringbuffer;
     int capturing;
     int capture_start;
     int capture_stop;
     int capture_data;
     process_callback_t process_callback;
     void *callback_arg;
};

audioclient* audioclient_create(void);
void audioclient_stop(audioclient *c);
void audioclient_free(audioclient *c);
void audioclient_set_process_callback(audioclient *c,
				      process_callback_t callback,
				      void *arg);
int audioclient_process(jack_nframes_t nframes, void *client);
void audioclient_process_capture(audioclient *c, audio_buffer *b);
void audioclient_shutdown(void *arg);
void audioclient_init(audioclient *c);
void audioclient_start_capture(audioclient *c);
void audioclient_stop_capture(audioclient *c);

#endif
