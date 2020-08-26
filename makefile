CXX ?= g++-9
CXXFLAGS ?= --std=c++2a -Wall --pedantic -pedantic-errors -O1

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<

all: spectrum

apt:
	apt update
	apt install --yes gnuplot

spectrum: spectrum.o fourier.o
	$(CXX) -o $@ $^

clean:
	rm -f *.o spectrum

noise: spectrum
	arecord -q -f S16_LE -c1 -r 8000 | ./spectrum
