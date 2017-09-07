#ifndef FOURIER_H
#define FOURIER_H

#include <vector>

// The number of bins is fundamental. It's the number of samples to read,
// size of the twiddle matrix and the resulting Fourier transform.
const unsigned int bins = 1000;
std::vector<double> fourier(const std::vector<short> &);

#endif
