#include "fourier.h"
#include "notes.h"
#include "riff.h"
#include <algorithm>
#include <iostream>

void chord();
void chord() try {

  using namespace std;

  const size_t bins = 1000;
  const auto samples = rif::read_samples(bins);
  const auto fou = fourier(samples);

  // Bin resolution
  const double bin_resolution =
      rif::header.sample_rate / static_cast<double>(samples.size());

  cout << "Bins " << bins << endl;
  cout << "Sample rate " << rif::header.sample_rate << " Hz" << endl;
  cout << "Bin resolution " << bin_resolution << " Hz" << endl;

  // ASCII representation of an octave on the piano
  const vector<string> keys = {
      " ", " ", "|", " ", "|", " ", " ", "|", " ", "|", " ", "|",
  };

  const unsigned long key_count = 74;
  const unsigned long notes_in_an_octave = 12;

  // Construct ASCII keyboard
  string keyboard;
  for (unsigned long i = 0; i < key_count; ++i)
    keyboard += keys.at(i % notes_in_an_octave);

  const auto hrule = string(key_count, '_');
  cout << hrule << endl;
  cout << keyboard << endl;
  cout << keyboard << endl;

  // Find the max element so we know how large the peaks will be
  const auto max_bin = *max_element(fou.cbegin(), fou.cend());

  // Pass over the results and calculate corresponding piano keys
  string key_strikes = string(key_count, ' ');
  const unsigned long window = 1;

  // Peak detector
  for (unsigned long bin = window; bin < fou.size() - window; ++bin) {

    const double bin_freq = bin * bin_resolution;

    const auto previous = fou.at(bin - window);
    const auto current = fou.at(bin);
    const auto next = fou.at(bin + window);

    // Store this bin if its value is over the threshold
    const double threshold = max_bin / 5;
    if (current > 0 && (current - previous) > threshold &&
        (current - next) > threshold) {

      // Find insertion point and key index for this note
      const auto note = --riff::notes.lower_bound(bin_freq);
      const unsigned long key =
          distance(riff::notes.cbegin(), note) % key_count;

      key_strikes.at(key) = '^';
    }
  }

  cout << hrule << endl;
  cout << key_strikes << endl;

} catch (const std::exception &e) {
  std::cout << "Caught " << e.what() << std::endl;
}

int main() {

  chord();
  return 0;
}
