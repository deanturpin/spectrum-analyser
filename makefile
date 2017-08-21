CC=clang++
STANDARD=c++14
FLAGS=-Weverything -O3 -Wno-c++98-compat -std=$(STANDARD)

%.o:%.cpp
	$(CC) $(FLAGS) -o $@ -c $<

all: fourier tonegen generate

fourier: fourier.o
tonegen: tonegen.o
generate: generate.o

clean:
	rm -f fourier tonegen generate *.o

# Analyse mic input
live: fourier
	watch -c -t -n .01 "arecord -q -f S16_LE -c1 -r 2000 | ./fourier"

# Analyse generated tone
demo: tonegen fourier
	./tonegen 20 30 40 | ./fourier | head -40

# Wait for a cpp to be updated and build
wait:
	while :; do inotifywait -qe modify *.cpp; make; done

progress:
	./tonegen 196.00 246.94 293.66 | ./fourier
	./tonegen 261.63 329.63 392.00 | ./fourier
	./tonegen 261.63 311.13 392.00 | ./fourier

cppcheck:
	cppcheck --enable=all .
