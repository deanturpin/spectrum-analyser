CC=g++
FLAGS=-std=c++17 -O3 -Wall --pedantic -I lib

%.o: %.cpp
	$(CC) $(FLAGS) -o $@ -c $<

# all: bin/spectrum

spectrum: spectrum.o fourier.o
	$(CC) -o $@ $^

clean:
	rm -f *.o spectrum

noise: spectrum
	arecord -q -f S16_LE -c1 -r 4000 | ./spectrum

# Lint
cppcheck:
	cppcheck --enable=all .

# In place format
format:
	$(foreach file, $(wildcard *.cpp), clang-format -i $(file) || true;)
	$(foreach file, $(wildcard *.h), clang-format -i $(file) || true;)
