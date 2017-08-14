#include "riff.h"
#include <vector>
#include <complex>
#include <iostream>
#include <algorithm>

int main() {

	using namespace std;

	// Read the WAV header
	riff::wav wav;
	cin.read(reinterpret_cast<char*>(&wav), sizeof wav);

	// Number of samples to read, size of matrix and resulting Fourier transform
	const unsigned int bins = 1000;

	// Read samples from stdin
	vector<short> X(bins);
	cin.read(reinterpret_cast<char*>(X.data()), bins * sizeof(short));

	// Create matrix
	auto *matrix = new complex<double>[bins][bins]();

	for (double k = 0.0; k < bins; ++k)
		for (double n = 0.0; n < bins; ++n)
			matrix[static_cast<int>(n)][static_cast<int>(k)]
				= exp(-2i * M_PI * k * n / static_cast<double>(bins)); // This is the magic

	// The resultant Fourier transform is the dot product of this matrix and the
	// original samples
	vector<complex<double>> fourier;
	for (unsigned int k = 0; k < bins; ++k) {

		complex<double> sum;
		for (unsigned int n = 0; n < bins; ++n)
			sum += matrix[n][k] * complex<double>(X[n], 0);	
	
		fourier.push_back(sum/static_cast<double>(bins));
	}

	delete [] matrix;

	// Bin resolution
	const double resolution = wav.sample_rate / bins;

	// Find the peak value. But only search the first half of the results. The
	// rest are essentially mirrored but may have a higher peak.
	const auto max = max_element(fourier.cbegin(), fourier.cbegin() + fourier.size()/2,
		[](const auto &a, const auto &b){ return abs(a) < abs(b); });

	// And the peak bin is the distance from the start to the max iterator
	const auto peakBin = static_cast<unsigned int>(distance(fourier.cbegin(), max));

	// Let's print the results
	cout << "Sample rate " << wav.sample_rate << " Hz" << endl;
	cout << "Resolution " << resolution << " Hz" << endl;
	cout << "Bins " << fourier.size() << endl;
	cout << "Peak " << peakBin << endl;
	cout << endl;
	cout << "Hertz" << endl;

	// Print the Fourier transform
	for (const auto &f : fourier) {

		static unsigned int bin = 0;

		// cout << static_cast<unsigned int>(bin * resolution) << "\t";

		const double fullScale = 0xffff;
		const auto length
			= static_cast<unsigned int>(1 + round(160 * (fullScale/4 + abs(f))/fullScale));

		// Print the line
		cout << "\033[33m" << string(length, '-') << "\033[0m|";

		// If we're on the peak bin top with a marker and the current bin frequency
		if (bin == peakBin) {

			cout << "> \033[41m:)\033[0m ";
			cout << static_cast<unsigned int>(bin * resolution);
		}

		cout << endl;

		++bin;
	}

	return 0;
}
