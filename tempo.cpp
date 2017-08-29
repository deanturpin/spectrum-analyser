#include "notes.h"
#include "riff.h"
#include <algorithm>
#include <iostream>

void tempo();
void tempo() try {

  using namespace std;


  cout << "TEMPO" << endl;
  cout << "Collecting ... " << endl;

  const unsigned int batch_size = 4000;
  auto samples = rif::read_samples(batch_size);
  cout << "Sample rate " << rif::header.sample_rate << " Hz" << endl;
  cout << "Samples " << samples.size() << endl;

  const auto minmax = minmax_element(samples.cbegin(), samples.cend());

  map<unsigned int, short> histogram;

  const unsigned int bins = 100;
  const unsigned int scale = batch_size  / bins;
  for (unsigned int i = 0; i < batch_size; ++i)
    ++histogram[i / scale] += samples.at(i);

  // for (const auto &i : histogram)
    // cout << i.first << "\t" << i.second << endl;

  // Peak detector
  const unsigned int window = 1;
  const auto threshold = *minmax.second / 5;

  for (unsigned int i = window; i < histogram.size() - window; ++i) {

    const auto previous = histogram.at(i - window);
    const auto current = histogram.at(i);
    const auto next = histogram.at(i + window);

    cout << current << "\t";
    cout << previous << "\t";
    cout << next << "\t";

    if (current > 0 && (current - previous) > threshold && (current - next) > threshold)
      cout << "*";

    cout << endl;
  }

} catch (const std::exception &e) {
  std::cout << "Caught " << e.what() << std::endl;
}

int main() {

  tempo();
  return 0;
}
