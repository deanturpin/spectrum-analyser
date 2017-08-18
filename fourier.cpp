#include "notes.h"
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

  // Read a batch of samples
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
  // original samples. Only run over the first half of the matrix as the other
  // half is a mirror image.
  vector<complex<double>> fourier;
  for (unsigned int k = 0; k < bins / 2; ++k) {

    complex<double> sum;
    for (unsigned int n = 0; n < bins; ++n)
      sum += twiddle[n][k] * complex<double>(samples.at(n), 0);

    // Store the average
    fourier.push_back(sum / static_cast<double>(bins));
  }

  // Free up the twiddles
  delete[] twiddle;

  // Bin resolution
  const double bin_resolution = wav.sample_rate / static_cast<double>(bins);

  // Print analysis summary
  cout << "Sample rate " << wav.sample_rate << " Hz" << endl;
  cout << "Bin resolution " << bin_resolution << " Hz" << endl;
  cout << "\nHertz" << endl;

  // Print the Fourier transform as an ASCII art histogram. Each bin is
  // converted into a bar.
  for (unsigned int bin = 0; bin < fourier.size(); ++bin) {

    const unsigned int bin_freq =
        static_cast<unsigned int>(round(bin * bin_resolution));

    // Normalise the results and scale to make the graph fit nicely into the
    // terminal. Note: the absolute value of the (complex) Fourier result is
    // used to calculate the bar length.
    const double full_scale = 0xffff;
    const double max_bar = 160;
    const double bar_offset = 30;
    const auto length = static_cast<unsigned int>(
        round(max_bar * (full_scale / 4 + abs(fourier.at(bin))) / full_scale) -
        bar_offset);

    // Print the bar and make it colourful
    const auto red = "\033[41m";
    const auto white = "\033[0m";
    const auto yellow = "\033[33m";
    cout << yellow << string(length, '-') << white << "| ";

    // Add a marker if it's interesting
    const double peak_threshold = 3500.0;
    if (abs(fourier.at(bin)) > peak_threshold) {

      // Calculate the note of this bin by searching for the current bin
      // frequency in the notes map. But use the note *preceding* the insertion
      // point returned by lower bound. Also nudge the bin frequency a
      // microtone, otherwise exact frequencies will be mapped to the previous
      // note.
      const auto note = --riff::notes.lower_bound(bin_freq + .01);
      cout << red << bin_freq << white << " " << note->second;
    }

    cout << endl;
  }

  return 0;
}
