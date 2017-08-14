CC=clang++
STANDARD=c++14
FLAGS=-Weverything -O2 -Wno-c++98-compat -std=$(STANDARD)

%.o:%.cpp
	$(CC) $(FLAGS) -o $@ -c $<

all: fourier

fourier: fourier.o

clean:
	rm -f fourier *.o

run:
	watch -t -n .01 "arecord -f cd | ./fourier"
