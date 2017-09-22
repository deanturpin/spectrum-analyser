#ifndef FOURIER_H
#define FOURIER_H

#include <vector>

void fourier_init();
std::vector<double> fourier(const std::vector<short> &);
const unsigned long fourier_bins = 1000;

#endif
