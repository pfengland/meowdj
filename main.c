#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include "dj.h"

volatile sig_atomic_t quit = 0;

void handle_signal(int signum) {
     printf("shutting down\n");
     quit = 1;
}

int main(int argc, char **argv) {

     dj *l = dj_create();
     dj_init(l);
     
     // handle quit signal
     signal(SIGINT, handle_signal);
     
     printf("ready\n");

     while (!quit && !l->w->quit) {
	  dj_process(l);
     }

     dj_stop(l);
     dj_free(l);

     return 0;
}
