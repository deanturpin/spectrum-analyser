#include <complex>
#include <iostream>

int main() {

  const unsigned long bins = 3000;

  using namespace std;

  cout << "#include <complex>\n";
  cout << "const unsigned long bins = " << bins << ";\n";
  cout << "const double twiddle_static[bins][bins][2] = {\n\n";

  for (unsigned long k = 0; k < bins; ++k) {

    cout << "\t{";
    for (unsigned long n = 0; n < bins; ++n) {

      const auto t = exp(complex<double>(0, 2) * M_PI * static_cast<double>(k) *
                         static_cast<double>(n) / static_cast<double>(bins));

      cout << "{" << t.real() << "," << t.imag() << "},";
    }

    cout << "},\n";
  }
  cout << "};\n";

  return 0;
}
