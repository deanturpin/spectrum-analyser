#include "fourier.h"
#include <chrono>
#include <complex>
#include <iostream>
#include <vector>

std::vector<double> fourier(const std::vector<short> &samples) {

  using namespace std;

  const auto timestamp_start = chrono::steady_clock::now();

  // Initialise twiddle matrix
  auto *twiddle = new complex<double>[bins][bins]();

  // Populate twiddle matrix. The "exp" is the important bit.
  for (unsigned int k = 0; k < bins; ++k)
    for (unsigned int n = 0; n < bins; ++n)
      twiddle[n][k] = exp(2i * M_PI * static_cast<double>(k) *
                          static_cast<double>(n) / static_cast<double>(bins));

  const auto timestamp_twiddle = chrono::steady_clock::now();

  // The Fourier transform is the dot product of the twiddle matrix and the
  // original samples. Only run over the first half of the matrix as the other
  // half is a mirror image.
  vector<double> results(bins / 2);
  for (unsigned long k = 0; k < results.size(); ++k) {
    for (unsigned long n = 0; n < bins; ++n) {

      std::complex<double> sum;
      for (unsigned int n = 0; n < bins; ++n)
        sum += twiddle[n][k] * std::complex<double>(samples.at(n), 0);

      results.at(k) = abs(sum);
    }
  }

  const auto timestamp_dot_product = chrono::steady_clock::now();
  cout << "Twid " << (timestamp_twiddle - timestamp_start).count() / 1e9
       << endl;
  cout << "Proc " << (timestamp_dot_product - timestamp_twiddle).count() / 1e9
       << endl;

  return results;
}
