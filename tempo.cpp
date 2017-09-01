#include "notes.h"
#include "riff.h"
#include <algorithm>
#include <iostream>

void tempo();
void tempo() try {

  using namespace std;

  cout << "TEMPO" << endl;
  cout << "Collecting ... " << endl;

  const unsigned long batch_size = 10000;
  const auto samples = rif::read_samples(batch_size);
  cout << "Sample rate " << rif::header.sample_rate << " Hz" << endl;
  cout << "Samples " << samples.size() << endl;

  const auto minmax = minmax_element(samples.cbegin(), samples.cend());

  map<unsigned long, short> histogram;

  const unsigned long bins = 100;
  const unsigned long scale = batch_size / bins;
  for (unsigned long i = 0; i < batch_size; ++i)
    ++histogram[i / scale] += samples.at(i);

  // Peak detector
  const unsigned long window = 1;
  const auto threshold = *minmax.second / 5;

  for (unsigned long i = window; i < histogram.size() - window; ++i) {

    const auto previous = histogram.at(i - window);
    const auto current = histogram.at(i);
    const auto next = histogram.at(i + window);

    cout << current << "\t";
    cout << previous << "\t";
    cout << next << "\t";

    if (current > 0 &&
        (current - previous) > threshold && (current - next) > threshold)
      cout << " #############";

    cout << endl;
  }

} catch (const std::exception &e) {
  std::cout << "Caught " << e.what() << std::endl;
}

int main() {

  tempo();
  return 0;
}
