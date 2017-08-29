#include "notes.h"
#include "riff.h"
#include "fourier.h"
#include <algorithm>
#include <iostream>

void chord();
void chord() try {

  using namespace std;

  const auto samples = rif::read_samples(jos::bins);
  const vector<double> fourier = jos::fourier(samples);

  // Bin resolution
  const double bin_resolution = rif::header.sample_rate /
    static_cast<double>(jos::bins);

  cout << "CHORD LORD" << endl;
  cout << "Bins " << jos::bins << endl;
  cout << "Sample rate " << rif::header.sample_rate << " Hz" << endl;
  cout << "Bin resolution " << bin_resolution << " Hz" << endl;

  // ASCII representation of an octave on the piano
  vector<string> keys = {
      " ", " ", "|", " ", "|", " ", " ", "|", " ", "|", " ", "|",
  };

  const unsigned int key_count = 74;
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

  // Pass over the results and calculate corresponding piano keys
  string key_strikes = string(key_count, ' ');
  const unsigned int window = 1;

  // Peak detector
  for (unsigned int bin = window; bin < fourier.size() - window; ++bin) {

    // const double current_bin = fourier.at(bin);
    const double bin_freq = bin * bin_resolution;
    const unsigned int threshold = max_bin / 5;

    const auto previous = fourier.at(bin - window);
    const auto current = fourier.at(bin);
    const auto next = fourier.at(bin + window);

    // Store this bin if its value is over the threshold
    if (current > 0
        && (current - previous) > threshold
        && (current - next) > threshold) {

      // Find insertion point and key index for this note
      const auto note = riff::notes.lower_bound(bin_freq);
      const unsigned int key = distance(riff::notes.cbegin(), note) % key_count;

      key_strikes.at(key) = '^';
    }
  }

  cout << string(key_count, '_') << endl;
  cout << key_strikes << endl;

} catch (const std::exception &e) {
  std::cout << "Caught " << e.what() << std::endl;
}

int main() {

  chord();
  return 0;
}