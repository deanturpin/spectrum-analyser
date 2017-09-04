#include "fourier.h"
#include "omp.h"
#include <chrono>
#include <complex>
#include <iostream>
#include <vector>

namespace jos {
std::vector<double> fourier(const std::vector<short> &samples) {

  using namespace std;

  auto start = chrono::steady_clock::now();

  // Initialise twiddle matrix
  auto *twiddle = new complex<double>[bins][bins]();

  // Populate twiddle matrix. The "exp" is the important bit.
#pragma omp parallel for
  for (unsigned int k = 0; k < bins; ++k)
#pragma omp parallel for
    for (unsigned int n = 0; n < bins; ++n)
      twiddle[n][k] =
        exp(complex<double>(0, 2) * M_PI * static_cast<double>(k) *
            static_cast<double>(n) / static_cast<double>(bins));

  // The Fourier transform is the dot product of the twiddle matrix and the
  // original samples. Only run over the first half of the matrix as the other
  // half is a mirror image.

  auto end = chrono::steady_clock::now();
  cout << "FT twiddle time " << (end - start).count() / 1e9 << endl;
  start = chrono::steady_clock::now();

  vector<double> results(bins / 2);

#pragma omp parallel for
  for (unsigned long k = 0; k < results.size(); ++k) {

    for (unsigned long n = 0; n < bins; ++n) {

      complex<double> sum;
      for (unsigned int n = 0; n < bins; ++n)
        sum += twiddle[n][k] * complex<double>(samples.at(n), 0);

      results.at(k) = abs(sum / static_cast<double>(bins));
    }

    // Store the absolute value of the complex average
    // results.at(k) = abs(sum / static_cast<double>(bins));
  }

  end = chrono::steady_clock::now();
  cout << "FT dot pro time " << (end - start).count() / 1e9 << endl;

  return results;
}
}
