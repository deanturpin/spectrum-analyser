FLAGS=--std=c++2a -Wall --pedantic -pedantic-errors -O1
CXX=g++

%.o: %.cpp
	$(CXX) $(FLAGS) -o $@ -c $<

all: spectrum

spectrum: spectrum.o fourier.o
	$(CXX) -o $@ $^

clean:
	rm -f *.o spectrum

noise: spectrum
	arecord -q -f S16_LE -c1 -r 8000 | ./spectrum
