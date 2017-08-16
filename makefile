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
	watch -c -t -n .01 "arecord -f S16_LE -c1 -r8000 | ./fourier"

# Analyse generated tone
analyse: tonegen fourier
	./tonegen | ./fourier | head -40

# Play a major scale
major: tonegen
	./tonegen 440 550 660 | aplay

# Wait for a cpp to be updated and build
wait:
	while :; do inotifywait -qe modify *.cpp; make; done

cppcheck:
	cppcheck --enable=all .
