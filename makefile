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

live: fourier
	watch -c -t -n .01 "arecord -f S16_LE -c1 -r44100 | ./fourier"

analyse: tonegen fourier
	./tonegen 440 | ./fourier | head -55

wait:
	$(shell while :; do inotifywait -qe modify *.cpp; make; done)
