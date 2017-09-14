C arrays are marginally quicker than vectors.

Calling reserve with a vector is quicker than a C array.

The twiddle array is the most computationally expensive procedure.

Only half the twiddle array is needed.

Aim for the compiler to do as much as possible up front: optimise away the
calculation.

Using floats for all the calculations is slower than doubles.

Creating the array offline results in a very quick twiddle calculation but it
takes a long time to compile and both clang and gcc cough for large arrays. The
build procedure is also becoming very complicated.
