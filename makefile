CC=clang++
STANDARD=c++11
FLAGS=-Weverything -O3 -Wno-c++98-compat -std=$(STANDARD)

%.o:%.cpp
	$(CC) $(FLAGS) -o $@ -c $<

all: chord spectrum tonegen tempo

chord: chord.o fourier.o
spectrum: spectrum.o fourier.o
tempo: tempo.o fourier.o
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
	# CPP
	$(foreach cpp, $(wildcard *.cpp), echo clang-format $(cpp) || true;)
	# Headers
	# clang-format $< > /tmp/blah.cpp; mv /tmp/blah.cpp $<
