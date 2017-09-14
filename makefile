CC=clang++
STANDARD=c++14
FLAGS=-Wall -O0 -Wpedantic -pedantic-errors -std=$(STANDARD) -I lib

%.o:%.cpp
	$(CC) $(FLAGS) -o $@ -c $<

all: bin/chord bin/spectrum bin/tony

bin/chord: app/chord.o lib/fourier.o
	$(CC) -o $@ $^

bin/spectrum: app/spectrum.o lib/fourier.o
	$(CC) -o $@ $^

bin/tony: app/tony.o
	$(CC) -o $@ $^

clean:
	rm -f bin/*
	rm -f lib/*.o
	rm -f app/*.o

# Wait for a cpp to be updated and build
wait:
	while :; do inotifywait -qe modify *.cpp; make; done

# Demos using generated tones
demo: bin/tony bin/chord bin/spectrum
	bin/tony 220 276 330 | bin/chord
	bin/tony 11.5 21 32 | bin/spectrum | head -40

# Demos using mix input
live-chord: chord
	watch -c -t -n .01 "arecord -q -f S16_LE -c1 -r 2000 | bin/chord"

live-spectrum: spectrum
	watch -c -t -n .01 "arecord -q -f S16_LE -c1 -r 22050 | bin/spectrum"

static:
	watch -c -t -n .01 "./spectrum < train_2000.wav"

live-tempo: tempo
	watch -c -t -n .01 "arecord -q -f S16_LE -c1 -r 2000 | bin/tempo"

# Lint
cppcheck:
	cppcheck --enable=all .

# In place format
format:
	$(foreach file, $(wildcard */*.cpp), clang-format -i $(file) || true;)
	$(foreach file, $(wildcard */*.h), clang-format -i $(file) || true;)
