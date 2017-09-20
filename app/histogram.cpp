#include "fourier.h"
#include "notes.h"
#include "riff.h"
#include <algorithm>
#include <bitset>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <vector>

void histogram();
void histogram() try {

  using namespace std;

  const unsigned long bins = 2000;

  // Read some samples
  const auto ts_start = chrono::steady_clock::now();
  const auto samples = read_samples(bins);
  const auto ts_read = chrono::steady_clock::now();
  cout << "Read " << (ts_read - ts_start).count() / 1e9 << endl;

  // Fourier analysis
  const auto fou = fourier(samples);
  const auto ts_fou = chrono::steady_clock::now();
  cout << "Tout " << (ts_fou - ts_start).count() / 1e9 << endl;

  // Bin resolution
  double bin_resolution = 1.0 * header.sample_rate / bins;

  cout << "Bins " << bins << " ";
  cout << quoted(bitset<16>(bins).to_string()) << endl;
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
      max_element(hist.cbegin(), hist.cend(), [](const auto &a, const auto &b) {
        return a.second < b.second;
      })->second;

  const auto white = "\033[0m";
  const auto yellow = "\033[33m";

  // Print a bar for each note
  for (auto i = hist.crbegin(); i != hist.crend(); ++i) {

    const auto ledger = distance(hist.crbegin(), i) % 2 == 1 ? "-" : " ";

    cout << ledger << " " << i->first << " " << yellow
         << string(80 * i->second / max_bin, '-') << white << endl;
  }

} catch (const std::exception &e) {
  std::cout << "Caught " << e.what() << std::endl;
}

int main() {

  histogram();
  return 0;
}
