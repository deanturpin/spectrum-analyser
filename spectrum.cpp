#include "fourier.h"
#include "notes.h"
#include "riff.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <unistd.h>
#include <vector>

int main() {

  try {

    using namespace std;

    const auto header = read_wav_header();
    fourier_init();

    vector<short> samples;
    while (read_samples(samples) > 0) {

      // Do fourier analysis and create a copy of the results to display them
      auto fou = fourier(samples);
      // Find the max element so we know how much to scale the results
      // const double max_bin = *max_element(display.cbegin(), display.cend());
      const double max_bin = *max_element(fou.cbegin(), fou.cend());

      // Print the Fourier transform as an ASCII art histogram. Each bin is
      // converted into a bar.
      static vector<unsigned long> display(fou.size());
      for (unsigned long i = 0; i < fou.size(); ++i) {

        // Normalise the results and scale to make the graph fit nicely into
        // the terminal
        const auto full_bar = 130.0;
        const auto bar_length =
            static_cast<unsigned long>(floor(full_bar * fou.at(i) / max_bin));

        // Display the largest value and decay if necessary
        const auto decay = 5ul;
        display.at(i) = max(display.at(i), bar_length);
        display.at(i) = display.at(i) > decay ? display.at(i) - decay : 0;
      }

      const double bin_resolution = 1.0 * header.sample_rate / fourier_bins;
      for_each(display.crbegin(), display.crend(),
               [&bin_resolution, &notes](const auto &i) {

                 const auto red = "\033[41m";
                 const auto white = "\033[0m";
                 const auto bin_freq = bin_resolution * (&i - display.data());
                 const auto note = notes.lower_bound(bin_freq);

                 cout << bin_freq << "\t" << string(i, '-') << red << "|" <<
                   white << " " << note->second << endl;
               });

      cout << "Bins " << fou.size() << endl;
      cout << "Resolution " << bin_resolution << endl;
      cout << "Sample rate " << header.sample_rate << " Hz" << endl;
    }
  } catch (const std::exception &e) {
    std::cout << "Caught " << e.what() << std::endl;
  }

  return 0;
}
