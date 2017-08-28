#include "fourier.h"
#include "notes.h"
#include "riff.h"
#include <algorithm>
#include <iostream>
#include <vector>

void spectrum();
void spectrum() try {

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

  vector<double> fourier;
  calculate_fourier(samples, fourier);

  // Bin resolution
  const double bin_resolution = wav.sample_rate / static_cast<double>(bins);

  cout << "SPECTRUM" << endl;
  cout << "Bins " << bins << endl;
  cout << "Sample rate " << wav.sample_rate << " Hz" << endl;
  cout << "Bin resolution " << bin_resolution << " Hz" << endl;

  // Find the max element so we know how much to scale the results
  const auto max_bin = *max_element(fourier.cbegin(), fourier.cend());

  // Print the Fourier transform as an ASCII art histogram. Each bin is
  // converted into a bar.
  for (unsigned int bin = 0; bin < fourier.size(); ++bin) {

    const double bin_freq = bin * bin_resolution;

    // Normalise the results and scale to make the graph fit nicely into the
    // terminal. The absolute value of the (complex) Fourier result is used to
    // calculate the bar length.
    const double full_bar = 75.0;
    const double current_bin = fourier.at(bin);
    const auto bar_length =
        static_cast<unsigned int>(round(full_bar * current_bin / max_bin));

    // Print the bar and make it colourful
    const auto red = "\033[41m";
    const auto white = "\033[0m";
    const auto yellow = "\033[33m";
    cout << yellow << string(bar_length, '-') << white << "| ";

    // Add a marker if the current bin has strong reponse
    if (current_bin > max_bin / 2) {

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
