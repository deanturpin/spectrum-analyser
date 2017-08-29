CC=g++
STANDARD=c++11
FLAGS=-Wall -O3 -Wno-c++98-compat -std=$(STANDARD) -fopenmp

%.o:%.cpp
	$(CC) $(FLAGS) -o $@ -c $<

all: chord spectrum tonegen tempo

chord_objects = chord.o fourier.o
chord: $(chord_objects)
	$(CC) -o $@ $(chord_objects) -lgomp

spectrum_objects = spectrum.o fourier.o
spectrum: $(spectrum_objects)
	$(CC) -o $@ $(spectrum_objects) -lgomp

tempo: tempo.o
tonegen: tonegen.o

clean:
	rm -f chord spectrum tempo tonegen *.o

# Analyse mic input
live-chord: chord
	watch -c -t -n .01 "arecord -q -f S16_LE -c1 -r 2000 | ./chord"

live-spectrum: spectrum
	watch -c -t -n .01 "arecord -q -f S16_LE -c1 -r 2000 | ./spectrum"

live-tempo: tempo
	watch -c -t -n .01 "arecord -q -f S16_LE -c1 -r 8000 | ./tempo"
wav:
	watch -c -t -n .01 "./tempo < ../__fourier/emaj.wav"

# Analyse generated tone
demo: tonegen chord spectrum
	./tonegen 220 275 330 | ./chord
	./tonegen 11 21 32 | ./spectrum | head -40

# Wait for a cpp to be updated and build
wait:
	while :; do inotifywait -qe modify *.cpp; make; done

progress: tonegen chord
	./tonegen 196.00 246.94 293.66 | aplay -q
	./tonegen 196.00 246.94 293.66 | ./chord
	./tonegen 261.63 329.63 392.00 | aplay -q
	./tonegen 261.63 329.63 392.00 | ./chord
	./tonegen 261.63 311.13 392.00 | aplay -q
	./tonegen 261.63 311.13 392.00 | ./chord

cppcheck:
	cppcheck --enable=all .

clang-format:
	$(foreach file, $(wildcard *.cpp), clang-format $(file) > blah; mv blah $(file) || true;)
	$(foreach file, $(wildcard *.h), clang-format $(file) > blah; mv blah $(file) || true;)
