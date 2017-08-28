#include "notes.h"
#include "riff.h"
#include <algorithm>
#include <complex>
#include <iostream>
#include <vector>

void fourier();

void fourier() try {

  using namespace std;

  // Read the WAV header
  riff::wav wav;
  cin.read(reinterpret_cast<char *>(&wav), sizeof wav);

  // The number of bins is fundamental. It's the number of samples to read,
  // size of the twiddle matrix and the resulting Fourier transform.
  const unsigned int bins = 2000;

  // Read a batch of samples
  vector<short> samples(bins);
  cin.read(reinterpret_cast<char *>(samples.data()), bins * sizeof(short));

  // Initialise twiddle matrix
  auto *twiddle = new complex<double>[bins][bins]();

  // Populate twiddle matrix. The "exp" is the important bit.
  for (unsigned int k = 0; k < bins; ++k)
    for (unsigned int n = 0; n < bins; ++n)
      twiddle[n][k] =
          exp(complex<double>(0, 2) * M_PI * static_cast<double>(k) *
              static_cast<double>(n) / static_cast<double>(bins));

  // The Fourier transform is the dot product of the twiddle matrix and the
  // original samples. Only run over the first half of the matrix as the other
  // half is a mirror image.
  vector<double> fourier;
  for (unsigned int k = 0; k < bins / 2; ++k) {

    complex<double> sum;
    for (unsigned int n = 0; n < bins; ++n)
      sum += twiddle[n][k] * complex<double>(samples.at(n), 0);

    // Store the absolute value of the complex average
    fourier.push_back(abs(sum / static_cast<double>(bins)));
  }

  // Free up the twiddles
  delete[] twiddle;

  // Bin resolution
  const double bin_resolution = wav.sample_rate / static_cast<double>(bins);

  cout << "CHORD LORD" << endl;
  cout << "Sample rate " << wav.sample_rate << " Hz" << endl;
  cout << "Bin resolution " << bin_resolution << " Hz" << endl;

  // ASCII representation of an octave on the piano
  vector<string> keys = {
      " ", " ", "|", " ", "|", " ", " ", "|", " ", "|", " ", "|",
  };

  const unsigned int key_count = 72;
  const unsigned int notes_in_an_octave = 12;

  // Construct ASCII keyboard
  string keyboard;
  for (unsigned int i = 0; i < key_count; ++i)
    keyboard += keys.at(i % notes_in_an_octave);

  cout << string(key_count, '_') << endl;
  cout << keyboard << endl;
  cout << keyboard << endl;

  // Find the max element so we know how large the peaks will be
  const auto max_bin = *max_element(fourier.cbegin(), fourier.cend());

  // Pass over the results and calculate corresponding piano key
  string key_strikes = string(key_count, ' ');
  map<string, bool> key_list;
  for (unsigned int bin = 0; bin < fourier.size(); ++bin) {

    const double current_bin = fourier.at(bin);
    const double bin_freq = bin * bin_resolution;

    if (current_bin > max_bin / 2) {

      const auto note = riff::notes.lower_bound(bin_freq);
      const auto key = distance(riff::notes.cbegin(), note) % key_count;

      key_strikes.at(key) = 'O';
      key_list[note->second] = true;
    }
  }

  cout << string(key_count, '_') << endl;
  cout << key_strikes << endl;

  // Dump the list of keys
  cout << "----" << endl;
  for (const auto &i : key_list)
    cout << i.first << endl;

} catch (const std::exception &e) {
  std::cout << "Caught " << e.what() << std::endl;
}

int main() {

  fourier();
  return 0;
}
