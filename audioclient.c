#include "audioclient.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

audioclient* audioclient_create(void) {
     audioclient *c = malloc(sizeof(*c));
     c->client = NULL;
     c->input_port = NULL;
     c->output_port1 = NULL;
     c->output_port2 = NULL;
     c->ringbuffer = NULL;
     c->capturing = 0;
     c->capture_stop = 0;
     c->capture_data = 0;
     c->process_callback = NULL;
     c->callback_arg = NULL;
     return c;
}

void audioclient_stop(audioclient *c) {
     jack_client_close(c->client);
}

void audioclient_free(audioclient *c) {
     if (c->ringbuffer)
	  jack_ringbuffer_free(c->ringbuffer);
     return;
}

void audioclient_set_process_callback(audioclient *c,
				     process_callback_t callback,
				     void *arg) {
     c->process_callback = callback;
     c->callback_arg = arg;
}

int audioclient_process(jack_nframes_t nframes, void *client) {

    // get the audio client arg
     audioclient *c = client;

    // capture samples to the ringbuffer
    jack_default_audio_sample_t *in;
    if (c->capturing) {
	in = jack_port_get_buffer(c->input_port, nframes);
        jack_ringbuffer_write(c->ringbuffer, (void *)in, nframes * sizeof(*in));
        c->capture_data = 1;
    }

    jack_default_audio_sample_t *out1;
    jack_default_audio_sample_t *out2;
    out1 = jack_port_get_buffer(c->output_port1, nframes);
    out2 = jack_port_get_buffer(c->output_port2, nframes);

    // call callback to get playback data
    if (c->process_callback) {
	 c->process_callback(c->callback_arg, nframes, in, out1, out2);
    } else {
	 for (int i=0; i<nframes; i++) {
	      out1[i] = 0;
	      out2[i] = 0;
	 }
    }
    
    return 0;
}

// copy the captured samples to our buffer
void audioclient_process_capture(audioclient *c, audio_buffer *buffer) {

    if (c->capture_start) {
        audio_buffer_free_samples(buffer);
	audio_buffer_init(buffer);
	c->capturing = 1;
	c->capture_start = 0;
    }
    if (c->capture_stop) {
	c->capturing = 0;
	c->capture_stop = 0;
    }

    int samplesize = sizeof(jack_default_audio_sample_t);
    int readsize = jack_ringbuffer_read_space(c->ringbuffer);

    if (c->capture_data && readsize >= samplesize) {

        // number of samples we can read
        int readcount = readsize / samplesize;
        int newcount = buffer->count + readcount;
	int buffsize = samplesize * newcount;

	if (buffer->samples) {
	    void *newmem = realloc(buffer->samples, buffsize);
	    buffer->samples = newmem;
	} else {
	    buffer->samples = malloc(buffsize);
	}
        if (!buffer->samples) {
            printf("error allocating buffer - out of memory?\n");
	    exit(1);
        }

        jack_ringbuffer_read(c->ringbuffer, 
                             (void*)&(buffer->samples[buffer->count]),
                             readcount * samplesize);
    
	buffer->count = newcount;
        buffer->start = 0;
        buffer->end = buffer->count;
    }
}

void audioclient_shutdown(void *arg) {
    printf("pausetape: jack shutdown\n");
    exit(1);
}

void audioclient_init(audioclient *c) {

    if ((c->client = jack_client_open("sampler", 0, NULL)) == 0) {
	printf("error creating jack client\n");
	exit(1);
    }

    jack_set_process_callback(c->client, audioclient_process, c);
    jack_on_shutdown(c->client, audioclient_shutdown, NULL);

    c->ringbuffer = jack_ringbuffer_create(sizeof(double) *
					   audioclient_ringbuffer_size);
    memset(c->ringbuffer->buf, 0, c->ringbuffer->size);

    if ((c->input_port = jack_port_register(c->client, "input",
					    JACK_DEFAULT_AUDIO_TYPE,
					    JackPortIsInput, 0)) == 0) {
	printf("can't register input port\n");
	jack_client_close(c->client);
	exit(1);
    }

    if ((c->output_port1 = jack_port_register(c->client, "output1",
					     JACK_DEFAULT_AUDIO_TYPE,
					     JackPortIsOutput, 0)) == 0) {
	printf("can't register output port\n");
	jack_client_close(c->client);
	exit(1);
    }
    if ((c->output_port2 = jack_port_register(c->client, "output2",
					      JACK_DEFAULT_AUDIO_TYPE,
					      JackPortIsOutput, 0)) == 0) {
	 printf("can't register output port\n");
	 jack_client_close(c->client);
	 exit(1);
    }

    // activate the port before we connect
    if (jack_activate(c->client)) {
	printf("error activating jack client\n");
    }

    // connect to the first physical input port
    /*    const char **ports;
    ports = jack_get_ports(c->client, NULL, NULL,
			   JackPortIsPhysical|JackPortIsOutput);
    if (ports == NULL) {
	printf("cannot find any physical capture ports\n");
	exit(1);
    }
    const char* portname = jack_port_name(c->input_port);
    int ret;
    if ((ret = jack_connect(c->client, ports[0], portname)) != 0) {
	printf("cannot connect input port\n");
    }
    free(ports);

    // connect to the first physical output port
    ports = jack_get_ports(c->client, NULL, NULL,
			   JackPortIsPhysical|JackPortIsInput);

    if (ports == NULL) {
	printf("cannot find any physical playback ports\n");
	exit(1);
    }
    portname = jack_port_name(c->output_port);
    if ((ret = jack_connect(c->client, portname, ports[0])) != 0) {
	printf("cannot connect output port\n");
    }

    free(ports); */
}

void audioclient_start_capture(audioclient *c) {
    c->capture_start = 1;
    printf("capturing\n");
}

void audioclient_stop_capture(audioclient *c) {
    c->capture_stop = 1;
}
