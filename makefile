CXX=g++-6
FLAGS=-std=c++1z -Wall --pedantic -g

%.o: %.cpp
	$(CXX) $(FLAGS) -o $@ -c $<

spectrum: spectrum.o fourier.o
	$(CC) -o $@ $^

clean:
	rm -f *.o spectrum

noise: spectrum
	arecord -q -f S16_LE -c1 -r 8000 | ./spectrum

# Lint
cppcheck:
	cppcheck --enable=all .

# In place format
format:
	$(foreach file, $(wildcard *.cpp), clang-format -i $(file) || true;)
	$(foreach file, $(wildcard *.h), clang-format -i $(file) || true;)
