#include "fourier.h"
#include <algorithm>
#include <chrono>
#include <complex>
#include <iostream>
#include <vector>

std::vector<double> fourier(const std::vector<short> &samples) {

  using namespace std;

  const auto ts_start = chrono::steady_clock::now();
  const auto bins = samples.size();

  // Initialise twiddle matrix
  vector<complex<double>> twiddle;
  twiddle.reserve(bins * bins);

  // Populate twiddle matrix
  for (unsigned int k = 0; k < bins / 2; ++k)
    for (unsigned int n = 0; n < bins; ++n) {

      const double _k = k;
      const double _n = n;
      const double _bins = bins;

      // Euler's magic
      twiddle[(k * bins) + n] = exp(2i * M_PI * _k * _n / _bins);
    }

  const auto ts_twiddle = chrono::steady_clock::now();

  // The Fourier transform is the dot product of the twiddle matrix and the
  // original samples. Only run over the first half of the matrix as the other
  // half is a mirror image.
  vector<double> fou(bins / 2);
  generate(
      fou.begin(), fou.end(), [&samples, &bins, &twiddle, k = 0ul ]() mutable {

        // Use transform/accumulate here?

        complex<double> sum;
        for (unsigned int n = 0; n < bins; ++n)
          sum += twiddle[(k * bins) + n] * complex<double>(samples.at(n), 0);

        ++k;
        return abs(sum);
      });

  const auto ts_dot_product = chrono::steady_clock::now();
  cout << "Twid " << (ts_twiddle - ts_start).count() / 1e9 << endl;
  cout << "Proc " << (ts_dot_product - ts_twiddle).count() / 1e9 << endl;

  return fou;
}
