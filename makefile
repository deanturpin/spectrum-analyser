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
	./tonegen 20 30 40 | ./fourier | head -40

# Wait for a cpp to be updated and build
wait:
	while :; do inotifywait -qe modify *.cpp; make; done

cppcheck:
	cppcheck --enable=all .
