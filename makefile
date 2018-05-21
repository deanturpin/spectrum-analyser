FLAGS=-g --coverage -std=c++14 -Wall --pedantic -pedantic-errors
CXX=clang++

%.o: %.cpp
	$(CXX) $(FLAGS) -o $@ -c $<

spectrum: spectrum.o fourier.o
	$(CXX) -o $@ $^ --coverage

clean:
	rm -f *.o spectrum

noise: spectrum
	arecord -q -f S16_LE -c1 -r 8000 | ./spectrum

format:
	clang-format -i *.h *.cpp
