CC=clang++
STANDARD=c++14
FLAGS=-Weverything -O3 -Wno-c++98-compat -std=$(STANDARD)

%.o:%.cpp
	$(CC) $(FLAGS) -o $@ -c $<

all: fourier tonegen

fourier: fourier.o
tonegen: tonegen.o

clean:
	rm -f tonegen fourier *.o

# Analyse mic input
live: fourier
	watch -c -t -n .01 "arecord -q -f S16_LE -c1 -r 2000 | ./fourier"

# Analyse generated tone
demo: tonegen fourier
	@echo Analyse generated chord
	./tonegen 79 100 123 | ./fourier | head -40

# Wait for a cpp to be updated and build
wait:
	while :; do inotifywait -qe modify *.cpp; make; done

progress: tonegen
	@echo Major to Minor
	./tonegen 196.00 246.94 293.66 | aplay
	./tonegen 261.63 329.63 392.00 | aplay
	./tonegen 261.63 311.13 392.00 | aplay

cppcheck:
	cppcheck --enable=all .
