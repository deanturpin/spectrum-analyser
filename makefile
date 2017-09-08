CC=clang++
STANDARD=c++14
FLAGS=-Wall -O3 -Wpedantic -pedantic-errors -std=$(STANDARD)

%.o:%.cpp
	$(CC) $(FLAGS) -o $@ -c $<

all: chord spectrum tonegen

chord: chord.o fourier.o
spectrum: spectrum.o fourier.o
tonegen: tonegen.o

clean:
	rm -f chord spectrum tonegen *.o

# Wait for a cpp to be updated and build
wait:
	while :; do inotifywait -qe modify *.cpp; make; done

# Demos using generated tones
demo: tonegen chord spectrum
	./tonegen 220 276 330 | ./chord
	./tonegen 11.5 21 32 | ./spectrum | head -40

# Demos using mix input
live-chord: chord
	watch -c -t -n .01 "arecord -q -f S16_LE -c1 -r 6000 | ./chord"

live-spectrum: spectrum
	watch -c -t -n .01 "arecord -q -f S16_LE -c1 -r 44100 | ./spectrum"

live-tempo: tempo
	watch -c -t -n .01 "arecord -q -f S16_LE -c1 -r 2000 | ./tempo"

# Lint
cppcheck:
	cppcheck --enable=all .

# In place format
format:
	$(foreach file, $(wildcard *.cpp), clang-format -i $(file) || true;)
