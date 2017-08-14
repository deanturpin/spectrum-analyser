#include <vector>
#include <complex>
#include <iostream>
#include <algorithm>

// The WAV format
namespace riff {

	struct wav {

		unsigned int riff_id;
		unsigned int riff_size;
		unsigned int wave_tag;
		unsigned int format_id;
		unsigned int format_size;
		unsigned int format_tag : 16;
		unsigned int channels : 16;
		unsigned int sample_rate;
		unsigned int bytes_per_second;
		unsigned int block_align : 16;
		unsigned int bit_depth : 16;
		unsigned int data_id;
		unsigned int data_size;
	};
}

int main() {

	using namespace std;

	// Read the WAV header
	riff::wav wav;
	cin.read(reinterpret_cast<char*>(&wav), sizeof wav);

	// Number of samples to read, size of matrix and resulting Fourier transform
	const unsigned int bins = 1470 / 4;

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

	// Let's print the results
	cout << "Sample rate " << wav.sample_rate << " Hz" << endl;
	cout << "Resolution " << resolution << " Hz" << endl;
	cout << "Bins " << fourier.size() << endl;
	cout << endl;
	cout << "Hertz" << endl;

	// Print the Fourier transform
	for (const auto &f : fourier) {

		static unsigned int bin = 0;

		cout << static_cast<unsigned int>(bin * resolution) << "\t";

		const double fullScale = 0xffff;
		const auto length = static_cast<unsigned int>(1 + round(160 * (fullScale/4 + abs(f))/fullScale));
		cout << string(length, ' ') << "|" << endl;

		++bin;

		if (bin > 52)
			break;
	}

	return 0;
}
