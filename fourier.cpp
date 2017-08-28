#include "notes.h"
#include "riff.h"
#include <algorithm>
#include <complex>
#include <iostream>
#include <iterator>
#include <limits>
#include <vector>

void fourier();

void fourier() try {

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
      twiddle[n][k] =
          exp(complex<double>(0, 2) * M_PI * static_cast<double>(k) *
              static_cast<double>(n) / static_cast<double>(bins));

  // Windowing
#if 0
  const unsigned int window_size = 100;
  for (unsigned int i = 0; i < window_size; ++i)
    samples.at(i) *= (i + 1) / window_size;

  for (unsigned int i = 0; i < window_size; ++i)
    samples.at(samples.size() - 1 - i) *= (i + 1) / window_size;
#endif

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

  cout << "Sample rate " << wav.sample_rate << " Hz" << endl;
  cout << "Bin resolution " << bin_resolution << " Hz" << endl;

  // Print some keys
  /*
  unsigned int keys = 0;
  for (const auto &key : riff::notes) {
    cout << "| " << key.second << "\t|" << endl;

    if (++keys > 12)
      break;
  }
  */

  const auto red = "\033[41m";
  const auto white = "\033[0m";
  const auto yellow = "\033[33m";

  // cout << yellow << "|    |" << endl;
  // cout << red << "| xxx|" << endl;
  // cout << yellow << "|    |" << endl;
  // cout << red << "| xxx|" << endl;
  // cout << yellow << "|    |" << endl;
  // cout << yellow << "|    |" << endl;
  // cout << red << "| xxx|" << endl;
  // cout << yellow << "|    |" << endl;
  // cout << red << "| xxx|" << endl;
  // cout << yellow << "|    |" << endl;
  // cout << red << "| xxx|" << endl;
  // cout << yellow << "|    |" << white << endl;

  /*
  vector<string> scale = {
    "C ",
    "C#",
    "D ",
    "D#",
    "E ",
    "F ",
    "F#",
    "G ",
    "G#",
    "A ",
    "A#",
    "B",
  };

  for (unsigned int i = 0; i < 24; ++i)
    cout << scale.at(i % 12) << " " << dec;

  cout << "|" << endl;
  */

  vector<string> keys = {
    " ",
    "|",
    " ",
    "|",
    " ",
    " ",
    "|",
    " ",
    "|",
    " ",
    "|",
    " ",
  };


  // Construct ASCII keyboard
  const unsigned int key_count = 72;
  string keyboard;
  for (unsigned int i = 0; i < key_count; ++i)
    keyboard += keys.at(i % 12);

  keyboard += "|";
  cout << string(key_count, '_') << endl;
  cout << keyboard << endl;
  cout << keyboard << " Chord Lord" << endl;

  // Find the max element so we know how much to scale the results
  // const auto max_bin = *max_element(fourier.cbegin(), fourier.cend());
  const auto max_bin = numeric_limits<short>::max() / 20;

  // Print the Fourier transform as an ASCII art histogram. Each bin is
  // converted into a bar.
  string key_strikes = string(key_count, ' ');
  for (unsigned int bin = 50; bin < fourier.size(); ++bin) {

    const double current_bin = fourier.at(bin);
    const double bin_freq = bin * bin_resolution;

    if (current_bin > max_bin / 2) {
      const auto note = riff::notes.lower_bound(bin_freq);
      const auto key = distance(riff::notes.cbegin(), note) % key_count;
      key_strikes.at(key - 1) = '^';
    }
  }

  cout << key_strikes << endl;
  cout << string(key_count, '_') << endl;

  for (unsigned int bin = 50; bin < fourier.size(); ++bin) {

    const double bin_freq = bin * bin_resolution;

    // Normalise the results and scale to make the graph fit nicely into the
    // terminal. The absolute value of the (complex) Fourier result is used to
    // calculate the bar length.
    const double full_bar = 75.0;
    const double current_bin = fourier.at(bin);
    const auto bar_length =
        static_cast<unsigned int>(round(full_bar * current_bin / max_bin));

    // Print the bar and make it colourful
    // cout << yellow << string(bar_length, '-') << white << "| ";

    // Add a marker if the current bin has strong reponse
    if (current_bin > max_bin / 2) {

      // Calculate the note of this bin by searching for the current bin
      // frequency in the notes map. But use the note *preceding* the insertion
      // point returned by lower bound. Also nudge the bin frequency a
      // microtone, otherwise exact frequencies will be mapped to the previous
      // note.
      const auto note = riff::notes.lower_bound(bin_freq);
      cout << red << bin_freq << white << " " << note->second << endl;
    }
  }

} catch (const std::exception &e) {
  std::cout << "Caught " << e.what() << std::endl;
}

int main() {

  fourier();
  return 0;
}
