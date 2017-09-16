#include "fourier.h"
#include "notes.h"
#include "riff.h"
#include <algorithm>
#include <bitset>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <vector>

void spectrum();
void spectrum() try {

  using namespace std;

  const unsigned long bins = 0b0000'0111'1101'0000;

  // Read some samples
  const auto ts_start = chrono::steady_clock::now();
  const auto samples = rif::read_samples(bins);
  const auto ts_read = chrono::steady_clock::now();
  cout << "Read " << (ts_read - ts_start).count() / 1e9 << endl;

  // Fourier analysis
  const auto fou = fourier(samples);
  const auto ts_fou = chrono::steady_clock::now();
  cout << "Tout " << (ts_fou - ts_start).count() / 1e9 << endl;

  // Bin resolution
  const double bin_resolution = 1.0 * rif::header.sample_rate / bins;

  cout << "Bins " << bins << " ";
  cout << quoted(bitset<16>(bins).to_string()) << endl;
  cout << "Sample rate " << rif::header.sample_rate << " Hz" << endl;
  cout << "Bin resolution " << bin_resolution << " Hz" << endl;

  // Find the max element so we know how much to scale the results
  const auto max_bin = *max_element(fou.cbegin(), fou.cend());

  // Print the Fourier transform as an ASCII art histogram. Each bin is
  // converted into a bar.
  const auto window = 1ul;
  for (unsigned long bin = window; bin < fou.size() - window; ++bin) {

    const double bin_freq = bin * bin_resolution;

    const auto previous = fou.at(bin - window);
    const auto current = fou.at(bin);
    const auto next = fou.at(bin + window);

    // Normalise the results and scale to make the graph fit nicely into the
    // terminal. The absolute value of the (complex) Fourier result is used to
    // calculate the bar length.
    const double full_bar = 75.0;
    const auto bar_length =
        max_bin > 0 ? (floor(full_bar * current / max_bin)) : 0;

    // Print the bar and make it colourful
    const auto red = "\033[41m";
    const auto white = "\033[0m";
    const auto yellow = "\033[33m";
    cout << yellow << string(bar_length, '-') << white << "| ";

    // Add a marker if the current bin has strong reponse
    const unsigned long threshold = max_bin / 5;
    if ((current - previous) > threshold && (current - next) > threshold) {

      // Calculate the note of this bin by searching for the current bin
      // frequency in the notes map. But use the note *preceding* the insertion
      // point returned by lower bound. Also nudge the bin frequency a
      // microtone, otherwise exact frequencies will be mapped to the previous
      // note.
      const auto note = --riff::notes.lower_bound(bin_freq);
      cout << red << bin_freq << white << " " << note->second;
    }

    cout << endl;
  }
} catch (const std::exception &e) {
  std::cout << "Caught " << e.what() << std::endl;
}

int main() {

  spectrum();
  return 0;
}
