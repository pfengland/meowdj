#include "midiclient.h"
#include <stdlib.h>

midiclient* midiclient_create(void) {
     midiclient *m = malloc(sizeof(*m));
     m->handle = NULL;
     m->portid = 0;
     m->queueid = 0;
     m->stop = 0;
     m->cc_callback = NULL;
     m->callback_arg = NULL;
     return m;
}

void midiclient_free(midiclient *m) {
     free(m);
}

void midiclient_stop(midiclient *m) {
     m->stop = 1;
     snd_seq_delete_simple_port(m->handle,m->portid);
     snd_seq_free_queue(m->handle, m->queueid);
     snd_seq_close(m->handle);
}

void midiclient_start(midiclient *m) {

     int err;
     err = snd_seq_open(&m->handle, "default", SND_SEQ_OPEN_INPUT, 0);
     if (err < 0) {
	  printf("error opening midi client\n");
	  exit(1);
     }
     
     snd_seq_set_client_name(m->handle, "MeowSampler");

     // create the input port which will show up in the patch bay
     m->portid = snd_seq_create_simple_port(m->handle, "my port",
					    SND_SEQ_PORT_CAP_WRITE|
					    SND_SEQ_PORT_CAP_SUBS_WRITE,
					    SND_SEQ_PORT_TYPE_MIDI_GENERIC);

     m->queueid = snd_seq_alloc_named_queue(m->handle, "sampler input queue");

     midiclient_startthread(m);
}

void midiclient_startthread(midiclient *m) {

     int status = pthread_create(&m->thread, NULL,
				 midiclient_thread, m);
     if (status == -1) {
	  printf("Unable to create MIDI input thread\n");
	  exit(1);
     }
}

void midiclient_read(midiclient *m) {
     snd_seq_event_t *ev = malloc(sizeof(*ev));
     while (snd_seq_event_input(m->handle, &ev) && !m->stop) {
	  printf("got event\n");
	  if (ev->type == SND_SEQ_EVENT_CONTROLLER) {
	       unsigned int param = ev->data.control.param;
	       unsigned int value = ev->data.control.value;
	       printf("control change: %d %d\n", param, value);

	       if (m->cc_callback) {
		    m->cc_callback(m->callback_arg, param, value);
	       }
	  }
     }
     free(ev);
}

void* midiclient_thread(void *m) {
     midiclient *c = m;
     while (!c->stop) {
	  midiclient_read(m);
     }
     return NULL;
}

void midiclient_set_cc_callback(midiclient *m, midiclient_callback_t callback,
				void *arg) {
     m->cc_callback = callback;
     m->callback_arg = arg;
}
