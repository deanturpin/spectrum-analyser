CC=g++
STANDARD=c++11
FLAGS=-Wall -Wpedantic -pedantic-errors -O4 -std=$(STANDARD) -fopenmp

%.o:%.cpp
	$(CC) $(FLAGS) -o $@ -c $<

all: twiddle.h chord spectrum tonegen tempo

chord_objects = chord.o fourier.o
chord: $(chord_objects)
	$(CC) -o $@ $(chord_objects) -lgomp

spectrum_objects = spectrum.o fourier.o
spectrum: $(spectrum_objects)
	$(CC) -o $@ $(spectrum_objects) -lgomp

tempo: tempo.o
tonegen: tonegen.o

fourier.o: generate_twiddle_matrix.o

generate_twiddle_matrix: generate_twiddle_matrix.o

twiddle.h: generate_twiddle_matrix
	./generate_twiddle_matrix > twiddle.h

clean:
	rm -f chord spectrum tempo tonegen generate_twiddle_matrix twiddle.h *.o

####################
# Wait for a cpp to be updated and build
####################

wait:
	while :; do inotifywait -qe modify *.cpp; make; done

####################
# Demos using generated tones
####################

demo: tonegen chord spectrum
	./tonegen 220 275 330 | ./chord
	./tonegen 11 21 32 | ./spectrum | head -40

####################
# Demos using mix input
####################

live-chord: chord
	watch -c -t -n .01 "arecord -q -f S16_LE -c1 -r 2000 | ./chord"

live-spectrum: spectrum
	watch -c -t -n .01 "arecord -q -f S16_LE -c1 -r 8000 | ./spectrum"

live-tempo: tempo
	watch -c -t -n .01 "arecord -q -f S16_LE -c1 -r 8000 | ./tempo"


####################
# Lint
####################

cppcheck:
	cppcheck --enable=all .

clang-format-cpp:
	$(foreach file, $(wildcard *.cpp), clang-format $(file) > blah; mv blah $(file) || true;)

clang-format-h:
	$(foreach file, $(wildcard *.h), clang-format $(file) > blah; mv blah $(file) || true;)
