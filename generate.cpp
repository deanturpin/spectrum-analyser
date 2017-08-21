#include "notes.h"
#include "riff.h"
#include <algorithm>
#include <complex>
#include <iostream>
#include <vector>

int main() {

  using namespace std;

  const unsigned int bins = 1000;

  cout << "#include <complex>" << endl;
  cout << "#include <vector>" << endl;
  cout << "" << endl;

  const string type =
    "const std::vector<std::vector<std::complex<double>>> fft_" + bins;

  cout << type << " = {\n" << endl;

  // Populate twiddle matrix. The "exp" is the important bit.
  for (unsigned int k = 0; k < bins; ++k) {

    cout << "\t{";

    for (unsigned int n = 0; n < bins; ++n) {

      const auto t = exp(-2i * M_PI * static_cast<double>(k) *
                         static_cast<double>(n) / static_cast<double>(bins));

      cout << "{" << t.real() << "," << t.imag() << "},";
    }

    cout << "}," << endl;
  }

  cout << "};" << endl;

  return 0;
}
