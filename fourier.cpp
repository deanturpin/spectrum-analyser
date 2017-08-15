#include "riff.h"
#include <algorithm>
#include <complex>
#include <iostream>
#include <vector>

int main() {

  using namespace std;

  // Read the WAV header
  riff::wav wav;
  cin.read(reinterpret_cast<char *>(&wav), sizeof wav);

  // The number of bins is fundamental. It's the number of samples to read,
  // size of the twiddle matrix and the resulting Fourier transform.
  const unsigned int bins = 1000;

  // Read samples from stdin
  vector<short> samples(bins);
  cin.read(reinterpret_cast<char *>(samples.data()), bins * sizeof(short));

  // Initialise twiddle matrix
  auto *twiddle = new complex<double>[bins][bins]();

  // Populate twiddle matrix. The "exp" is the important bit.
  for (unsigned int k = 0; k < bins; ++k)
    for (unsigned int n = 0; n < bins; ++n)
      twiddle[n][k] = exp(-2i * M_PI * static_cast<double>(k) *
                          static_cast<double>(n) / static_cast<double>(bins));

  // The Fourier transform is the dot product of the twiddle matrix and the
  // original samples
  vector<complex<double>> fourier;
  for (unsigned int k = 0; k < bins; ++k) {

    complex<double> sum;
    for (unsigned int n = 0; n < bins; ++n)
      sum += twiddle[n][k] * complex<double>(samples.at(n), 0);

    fourier.push_back(sum / static_cast<double>(bins));
  }

  // Free up the twiddles
  delete[] twiddle;

  // Bin resolution
  const double resolution = wav.sample_rate / bins;

  // Find the peak value but only search the first half of the results. The top
  // half is essentially a mirror image but may have a higher peak.
  const auto max =
    max_element(fourier.cbegin(), fourier.cbegin() + fourier.size() / 2,
                [](const auto &a, const auto &b) { return abs(a) < abs(b); });

  // The peak bin is the distance from the beginning of the Fourier transform
  // to the max iterator
  const auto peakBin =
    static_cast<unsigned int>(distance(fourier.cbegin(), max));

  // Print analysis summary
  cout << "Sample rate " << wav.sample_rate << " Hz" << endl;
  cout << "Resolution " << resolution << " Hz" << endl;
  cout << "Bins " << fourier.size() << endl;
  cout << "\nHertz" << endl;

  // Print resultant Fourier transform
  for (const auto &f : fourier) {

    static unsigned int bin = 0;

    // Normalise the results and scale to make the graph fit nicely into the
    // terminal. Note: the absolute value of the (complex) Fourier result is
    // used to calculate the bar length.
    const double fullScale = 0xffff;
    const double maxBar = 160;
    const double barOffset = 30;
    const auto length = static_cast<unsigned int>(
        round(maxBar * (fullScale / 4 + abs(f)) / fullScale) - barOffset);

    // Print the bar, make it colourful
    cout << "\033[33m" << string(length, '-') << "\033[0m|";

    // If we're at the peak, top the line with a marker and current bin
    // frequency
    if (bin == peakBin)
      cout << " \033[41m"
        << static_cast<unsigned int>(bin * resolution) << "\033[0m";

    cout << endl;

    ++bin;
  }

  return 0;
}
