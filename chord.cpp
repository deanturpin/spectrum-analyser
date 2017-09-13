#include "fourier.h"
#include "notes.h"
#include "riff.h"
#include <algorithm>
#include <iostream>

void chord();
void chord() try {

  using namespace std;

  const size_t bins = 0b0000'1000'0000'0000;
  const auto samples = rif::read_samples(bins);
  const auto fou = fourier(samples);

  // Bin resolution
  const double bin_resolution =
      rif::header.sample_rate / static_cast<double>(samples.size());

  cout << "Bins " << bins << endl;
  cout << "Sample rate " << rif::header.sample_rate << " Hz" << endl;
  cout << "Bin resolution " << bin_resolution << " Hz" << endl;

  // Length of the complete keyboard
  const unsigned long key_count = 74;

  // Initialise ASCII keyboard and populate it with a lambda expression. I've
  // given the capture variable an unusual name to highlight the fact that it
  // hasn't been declared beforehand. I've also used decltype instead of the
  // much simpler auto.
  string keyboard(key_count, '*');
  for_each(keyboard.begin(), keyboard.end(),
           [_x = 0ul](decltype(*keyboard.begin()) &key) mutable {

             // ASCII representation of a piano octave. Note that it starts on
             // a B because notes.h does.
             const string octave{"  | |  | | |"};

             // Write the current key character
             key = octave.at(_x++ % octave.size());
           });

  // Display the keyboard
  const auto hrule = string(key_count, '_');
  cout << hrule << endl;
  cout << keyboard << endl;
  cout << keyboard << endl;

  // Initialise key strikes
  string key_strikes = string(key_count, ' ');
  const unsigned long window = 1;

  // Peak detector - pass over the results and calculate corresponding keys
  for (unsigned long bin = window; bin < fou.size() - window; ++bin) {

    const double bin_freq = bin * bin_resolution;

    const auto previous = fou.at(bin - window);
    const auto current = fou.at(bin);
    const auto next = fou.at(bin + window);

    // Store this bin if its value is over the threshold
    const auto max_bin = *max_element(fou.cbegin(), fou.cend());
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
