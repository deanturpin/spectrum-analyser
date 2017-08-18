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
	watch -c -t -n .01 "arecord -f S16_LE -c1 -r 2000 | ./fourier"

# Analyse generated tone
demo: tonegen fourier
	./tonegen 118 | ./fourier | head -40

# Analyse a major chord
major: tonegen
	./tonegen 440 554 659 | ./fourier

# Wait for a cpp to be updated and build
wait:
	while :; do inotifywait -qe modify *.cpp; make; done

# Comparison of just intonation and equal temperament
just: tonegen fourier
	./tonegen 440 554 659 | aplay
	./tonegen 440 550 660 | aplay
	./tonegen 440 488 660 | aplay
	./tonegen 550 660 825 | aplay

cppcheck:
	cppcheck --enable=all .
