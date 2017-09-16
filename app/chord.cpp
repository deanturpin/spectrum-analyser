#include "fourier.h"
#include "notes.h"
#include "riff.h"
#include <algorithm>
#include <chrono>
#include <iostream>

void chord();
void chord() try {

  using namespace std;

  const size_t bins = 2000;

  const auto ts_start = chrono::steady_clock::now();
  const auto samples = read_samples(bins);
  const auto ts_read = chrono::steady_clock::now();
  cout << "Read " << (ts_read - ts_start).count() / 1e9 << endl;

  // Fourier analysis
  const auto fou = fourier(samples);
  const auto ts_fou = chrono::steady_clock::now();
  cout << "Tout " << (ts_fou - ts_start).count() / 1e9 << endl;

  // Bin resolution
  const double bin_resolution = 1.0 * header.sample_rate / samples.size();

  cout << "Bins " << bins << endl;
  cout << "Sample rate " << header.sample_rate << " Hz" << endl;
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
    const auto threshold = max_bin / 5;

    if (current > 0 && (current - previous) > threshold &&
        (current - next) > threshold) {

      // Find insertion point and key index for this note
      const auto note = notes.lower_bound(bin_freq);
      const auto key =
          static_cast<unsigned long>(distance(notes.cbegin(), note)) %
          key_count;

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
