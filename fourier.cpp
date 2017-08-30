#include "fourier.h"
#include "twiddle.h"
#include "omp.h"
#include <chrono>
#include <complex>
#include <iostream>
#include <vector>

namespace jos {
std::vector<double> fourier(const std::vector<short> &samples) {

  using namespace std;

  // The twiddle matrix is generated statically and included above

  // The Fourier transform is the dot product of the twiddle matrix and the
  // original samples. Only run over the first half of the matrix as the other
  // half is a mirror image.

  chrono::time_point<std::chrono::steady_clock> start = chrono::steady_clock::now();

  vector<double> results(bins / 2);

#pragma omp parallel for
  for (unsigned long k = 0; k < results.size(); ++k) {

    complex<double> sum;

#pragma omp parallel for
    for (unsigned long n = 0; n < bins; ++n) {

      const double real = twiddle_static[n][k][0];
      const double imag = twiddle_static[n][k][1];

      const complex<double> t(real, imag);

      sum += t * complex<double>(samples.at(n), 0);
    }

    // Store the absolute value of the complex average
    results.at(k) = abs(sum / static_cast<double>(bins));
  }

  chrono::time_point<std::chrono::steady_clock> end = chrono::steady_clock::now();
  cout << "FT dot pro time " << (end - start).count() / 1e9 << endl;

  return results;
}
}
