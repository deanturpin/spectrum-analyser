#include "fourier.h"
#include <algorithm>
#include <complex>
#include <iostream>
#include <vector>

std::vector<std::complex<double>> twiddle;

void fourier_init() {

  using namespace std;

  const auto bins = fourier_bins;
  twiddle.reserve(bins * bins);

  // Populate twiddle matrix - Euler's magic
  for (unsigned int k = 0; k < bins / 2; ++k)
    for (unsigned int n = 0; n < bins; ++n)
      twiddle.push_back(exp(2i * M_PI * double(k) * double(n) / double(bins)));
}

std::vector<double> fourier(const std::vector<short> &samples) {

  using namespace std;

  const auto bins = fourier_bins;

  // The Fourier transform is the dot product of the twiddle matrix and the
  // original samples. Only run over the first half of the matrix as the other
  // half is a mirror image.
  vector<double> fou(bins / 2);
  generate(fou.begin(), fou.end(), [&, k = 0ul]() mutable {
    complex<double> sum;
    for (unsigned int n = 0; n < bins; ++n)
      sum += twiddle[(k * bins) + n] * complex<double>(samples.at(n), 0);

    ++k;
    return abs(sum);
  });

  return fou;
}
