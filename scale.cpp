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
  const unsigned int bins = 2000;

  // Initialise twiddle matrix
  auto *twiddle = new complex<double>[bins][bins]();

  // Populate twiddle matrix. The "exp" is the important bit.
  for (unsigned int k = 0; k < bins / 2; ++k)
    for (unsigned int n = 0; n < bins; ++n)
      twiddle[n][k] = exp(-2i * M_PI * static_cast<double>(k) *
                          static_cast<double>(n) / static_cast<double>(bins));

  // Read a batch of samples
  vector<short> samples(bins);
  while(cin.read(reinterpret_cast<char *>(samples.data()), bins * sizeof(short))) {

    static unsigned int peaks = 0;

    cout << endl;

    // The Fourier transform is the dot product of the twiddle matrix and the
    // original samples. Only run over the first half of the matrix as the other
    // half is a mirror image.
    vector<complex<double>> fourier;
    for (unsigned int k = 0; k < bins; ++k) {

      complex<double> sum;
      for (unsigned int n = 0; n < bins; ++n)
        sum += twiddle[n][k] * complex<double>(samples.at(n), 0);

      // Store the average
      fourier.push_back(sum / static_cast<double>(bins));
    }

    // Bin resolution
    const double bin_resolution = wav.sample_rate / static_cast<double>(bins);

    // Print analysis summary
    // cout << "Sample rate " << wav.sample_rate << " Hz" << endl;
    // cout << "Bin resolution " << bin_resolution << " Hz" << endl;
    // cout << "\nHertz" << endl;

    // Print the Fourier transform as an ASCII art histogram. Each bin is
    // converted into a bar.
    map<unsigned int, unsigned int> intervals;
    string key_note = "";
    for (unsigned int bin = 10; bin < fourier.size(); ++bin) {

      const auto bin_freq = bin * bin_resolution;

      // Normalise the results and scale to make the graph fit nicely into the
      // terminal. Note: the absolute value of the (complex) Fourier result is
      // used to calculate the bar length.
      // const double full_scale = 0xffff;
      // const double max_bar = 160;
      // const double bar_offset = 30;
      // const auto length = static_cast<unsigned int>(
        // round(max_bar * (full_scale / 4 + abs(fourier.at(bin))) / full_scale) -
        // bar_offset);

      // Print the bar and make it colourful
      const auto red = "\033[41m";
      const auto white = "\033[0m";
      const auto yellow = "\033[33m";
      // cout << yellow << string(length, '-') << white << "| ";

      // Add a marker if it's interesting
      const double peak_threshold = 3000.0;
      if (abs(fourier.at(bin)) > peak_threshold) {

        // Calculate the note of this bin by searching for the current bin
        // frequency in the notes map. But use the note *preceding* the insertion
        // point returned by lower bound. Also nudge the bin frequency a
        // microtone, otherwise exact frequencies will be mapped to the previous
        // note.
        const auto note = --riff::notes.lower_bound(bin_freq + .01);
        static const auto first_note = distance(riff::notes.cbegin(), note);
        if (key_note == "")
          key_note = note->second;

        // cout << red << bin_freq << white << " " << note->second;
        const unsigned int interval =
          1 + ((distance(riff::notes.cbegin(), note) - first_note) % 12);

        // cout << note->second << "\t" << red << yellow << bin_freq
          // << " Hz\t" << white << " " << bin << " "
          // << interval << endl;

        ++intervals[interval];
      }

      // cout << endl;
    }

    // cout << "\nScale" << endl;
    cout << key_note << endl;
    // if (intervals.find(1) != intervals.cend())
      // cout << intervals.find(1)->second << endl;
    if (intervals.find(4) != intervals.cend()) cout << "Minor" << endl;
    if (intervals.find(5) != intervals.cend()) cout << "Major" << endl;
    if (intervals.find(8) != intervals.cend()) cout << "5th" << endl;
    if (intervals.find(10) != intervals.cend()) cout << "7th" << endl;

    // Print intervals
    // cout << "\nIntervals" << endl;
    // for (const auto &i : intervals)
      // cout << i.first << "\t" << i.second << endl;

    // fourier.clear();
    // cout << "End of PASS " << peaks << endl;
    ++peaks;

    if (peaks > 0)
      break;

  }

  // Free up the twiddles
  delete[] twiddle;

  // cout << "FINISHED" << endl;

  return 0;
}
