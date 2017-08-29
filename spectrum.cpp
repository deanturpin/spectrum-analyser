#include "notes.h"
#include "riff.h"
#include "fourier.h"
#include <algorithm>
#include <iostream>
#include <vector>

void spectrum();
void spectrum() try {

  using namespace std;

  const auto samples = rif::read_samples(jos::bins);
  const auto fourier = jos::fourier(samples);

  // Bin resolution
  const double bin_resolution = rif::header.sample_rate /
    static_cast<double>(jos::bins);

  cout << "SPECTRUM" << endl;
  cout << "Bins " << jos::bins << endl;
  cout << "Sample rate " << rif::header.sample_rate << " Hz" << endl;
  cout << "Bin resolution " << bin_resolution << " Hz" << endl;

  // Find the max element so we know how much to scale the results
  const auto max_bin = *max_element(fourier.cbegin(), fourier.cend());

  // Print the Fourier transform as an ASCII art histogram. Each bin is
  // converted into a bar.
  const unsigned int window = 1;
  for (unsigned int bin = window; bin < fourier.size() - window; ++bin) {

    const double bin_freq = bin * bin_resolution;

    const auto previous = fourier.at(bin - window);
    const auto current = fourier.at(bin);
    // const double current_bin = fourier.at(bin);
    const auto next = fourier.at(bin + window);

    // Normalise the results and scale to make the graph fit nicely into the
    // terminal. The absolute value of the (complex) Fourier result is used to
    // calculate the bar length.
    const double full_bar = 75.0;
    const auto bar_length =
        static_cast<unsigned int>(round(full_bar * current / max_bin));

    // Print the bar and make it colourful
    const auto red = "\033[41m";
    const auto white = "\033[0m";
    const auto yellow = "\033[33m";
    cout << yellow << string(bar_length, '-') << white << "| ";

    // Add a marker if the current bin has strong reponse
    const unsigned int threshold = max_bin / 10;
    if ((current - previous) > threshold
        && (current - next) > threshold) {

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
} catch (const std::exception &e) {
  std::cout << "Caught " << e.what() << std::endl;
}

int main() {

  spectrum();
  return 0;
}
