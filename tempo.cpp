#include "notes.h"
#include "riff.h"
// #include "fourier.h"
#include <algorithm>
#include <iostream>

void tempo();
void tempo() try {

  using namespace std;

  const auto samples = rif::read_samples(8000);

  cout << "TEMPO" << endl;
  cout << "Sample rate " << rif::header.sample_rate << " Hz" << endl;
  cout << "Samples " << samples.size() << endl;

  // for (const auto &i : samples)
    // cout << i << endl;

  const auto minmax = minmax_element(samples.cbegin(), samples.cend());

  cout << *minmax.first << endl;
  cout << *minmax.second << endl;

} catch (const std::exception &e) {
  std::cout << "Caught " << e.what() << std::endl;
}

int main() {

  tempo();
  return 0;
}
