CC=clang++
STANDARD=c++14
FLAGS=-Wall -O0 -Wpedantic -pedantic-errors -std=$(STANDARD) -I lib

%.o:%.cpp
	$(CC) $(FLAGS) -o $@ -c $<

all: chord spectrum tony

chord: chord.o fourier.o
spectrum: spectrum.o fourier.o
tony: tony.o

clean:
	rm -f chord spectrum tony *.o

# Wait for a cpp to be updated and build
wait:
	while :; do inotifywait -qe modify *.cpp; make; done

# Demos using generated tones
demo: tony chord spectrum
	./tony 220 276 330 | ./chord
	./tony 11.5 21 32 | ./spectrum | head -40

# Demos using mix input
live-chord: chord
	watch -c -t -n .01 "arecord -q -f S16_LE -c1 -r 2000 | ./chord"

live-spectrum: spectrum
	watch -c -t -n .01 "arecord -q -f S16_LE -c1 -r 22050 | ./spectrum"

static:
	watch -c -t -n .01 "./spectrum < train_2000.wav"

live-tempo: tempo
	watch -c -t -n .01 "arecord -q -f S16_LE -c1 -r 2000 | ./tempo"

# Lint
cppcheck:
	cppcheck --enable=all .

# In place format
format:
	$(foreach file, $(wildcard *.cpp), clang-format -i $(file) || true;)
	$(foreach file, $(wildcard *.h), clang-format -i $(file) || true;)
