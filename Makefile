TARGET=dj
OBJECTS=main.o midiclient.o audio_buffer.o audioclient.o dj.o window.o waveview.o library.o session.o libraryview.o dynamicarray.o songlist.o song.o scrollbar.o slider.o deck.o
CFLAGS=-Wall -g --std=gnu99 `sdl-config --cflags`
LDLIBS=-ljack -lasound -lpthread -lSDL -lX11 -lSDL_ttf -lsndfile
CC=gcc

$(TARGET) : $(OBJECTS)

clean:
	rm -rf $(OBJECTS) $(TARGET)
