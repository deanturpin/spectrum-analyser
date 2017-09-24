#include "fourier.h"
#include "notes.h"
#include "riff.h"
#include <algorithm>
#include <iostream>
#include <vector>

int main() {

  try {

    using namespace std;

    // Read some samples
    const auto header = read_wav_header();
    fourier_init();

    vector<short> samples;
    while (read_samples(samples) > 0) {

      const unsigned long bins = samples.size();
      const vector<double> fou = fourier(samples);

      // Bin resolution
      double bin_resolution = 1.0 * header.sample_rate / bins;
      cout << "Sample rate " << header.sample_rate << " Hz" << endl;
      cout << "Bin resolution " << bin_resolution << " Hz" << endl;

      // Create a histogram from the Fouerier results. Consolidate all bins into
      // frequencies that map onto a piano keyboard.
      map<string, unsigned long> hist;
      for (const auto &f : fou) {

        const auto bin = &f - fou.data();
        const double bin_freq = bin * bin_resolution;
        const auto note = notes.lower_bound(bin_freq);

        // Calculate the note/octave and drop the octave. All the octaves should
        // end up in the same bin. We're even not bothered about sharps at the
        // moment.
        string key = note->second;
        hist[key.erase(1, -1)] += f;
      }

      // Find the max element so we know how much to scale the results
      const double max_bin =
          max_element(hist.cbegin(), hist.cend(), [](const auto &a,
                                                     const auto &b) {
            return a.second < b.second;
          })->second;

      // Print a bar for each note
      for (auto i = hist.crbegin(); i != hist.crend(); ++i) {

        const auto ledger = distance(hist.crbegin(), i) % 2 == 1 ? "-" : " ";

        cout << ledger << " " << i->first << " "
             << string(80 * i->second / max_bin, '-') << endl;
      }
    }
  } catch (const std::exception &e) {
    std::cout << "Caught " << e.what() << std::endl;
  }

  return 0;
}
