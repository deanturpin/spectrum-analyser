CC=clang++
STANDARD=c++1y
FLAGS=-Weverything -O2 -Wno-c++98-compat -std=$(STANDARD)

%.o:%.cpp
	$(CC) $(FLAGS) -o $@ -c $<

all: zoe dft matrix

zoe: zoe.o
dft: dft.o
matrix: matrix.o

clean:
	rm -f zoe dft matrix *.o

run:
	watch -t -n .01 -c "arecord -f cd | ./dft"
