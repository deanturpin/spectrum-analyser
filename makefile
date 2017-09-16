CC=clang++
FLAGS=-std=c++14 -O -Weverything -I lib -Wno-c++98-compat -Wno-c++98-c++11-compat-pedantic

%.o:%.cpp
	$(CC) $(FLAGS) -o $@ -c $<

all: bin/chord bin/spectrum bin/tony bin/histogram

bin/chord: app/chord.o lib/fourier.o
	$(CC) -o $@ $^

bin/spectrum: app/spectrum.o lib/fourier.o
	$(CC) -o $@ $^

bin/histogram: app/histogram.o lib/fourier.o
	$(CC) -o $@ $^

bin/tony: app/tony.o
	$(CC) -o $@ $^

clean:
	rm -f bin/*
	rm -f lib/*.o
	rm -f app/*.o

# Wait for a cpp to be updated and build
wait:
	while :; do inotifywait -qe modify */*.cpp; make; done

# Demos using generated tones
demo: bin/tony bin/chord bin/spectrum
	bin/tony 220 276 330 | bin/chord
	bin/tony 10 21 32 | bin/spectrum | head -40

# Demos using mix input
live-chord: bin/chord
	watch -t -n .01 "arecord -q -f S16_LE -c1 -r 4000 | bin/chord"

live-spectrum: bin/spectrum
	watch -c -t -n .01 "arecord -q -f S16_LE -c1 -r 8000 | bin/spectrum"

live-histogram: bin/histogram
	watch -t -n .01 "arecord -q -f S16_LE -c1 -r 8000 | bin/histogram"

static:
	watch -c -t -n .01 "bin/spectrum < wav/train_2000.wav"

live-tempo: bin/tempo
	watch -t -n .01 "arecord -q -f S16_LE -c1 -r 2000 | bin/tempo"

# Lint
cppcheck:
	cppcheck --enable=all .

# In place format
format:
	$(foreach file, $(wildcard */*.cpp), clang-format -i $(file) || true;)
	$(foreach file, $(wildcard */*.h), clang-format -i $(file) || true;)
