#include <map>
#include <math.h>
#include <limits>
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
		unsigned int data_id;
		unsigned int data_size;
	};
}

int main() {

	using namespace std;

	// Read the header
	riff::wav wav;
	cin.read(reinterpret_cast<char*>(&wav), sizeof wav);

	const unsigned int bins = 44; // 1470 / 2;
	vector<short> samples(bins);
	cin.read(reinterpret_cast<char*>(samples.data()), bins * sizeof(short));

	// Normalise samples
	vector<double> X(samples.cbegin(), samples.cend());
	// for (auto &i : X)
		// i /= numeric_limits<short>::max();

	// N is the number of samples
	const double N = X.size();

	auto *matrix = new complex<double>[bins][bins]();

	// Generate a matrix of exponents
	for (double k = 0.0; k < X.size(); ++k)
		for (double n = 0.0; n < X.size(); ++n)
			matrix[static_cast<int>(n)][static_cast<int>(k)]
				= exp(-2i * M_PI * k * n / N);

	vector<complex<double>> fourier;

	for (unsigned int k = 0; k < X.size(); ++k) {

		complex<double> sum;

		for (unsigned int n = 0; n < X.size(); ++n)
			sum += matrix[k][n] * complex<double>(X[n], 0);	
	
		fourier.push_back(sum/N);
	}

	delete [] matrix;

	const auto max = max_element(fourier.cbegin(), fourier.cend(),
			[](const auto &a, const auto &b){ return a.real() < b.real(); });

	const auto min = min_element(fourier.cbegin(), fourier.cend(),
			[](const auto &a, const auto &b){ return a.real() < b.real(); });

	const double resolution = wav.sample_rate / bins;

	cout << "Channels " << wav.channels << endl;
	cout << "Sample rate " << wav.sample_rate << " Hz" << endl;
	cout << "Resolution " << resolution << " Hz" << endl;
	cout << "Fourier bins " << fourier.size() << endl;
	cout << "Min " << *min << endl;
	cout << "Max " << *max << endl;
	cout << "Max bin " << distance(fourier.cbegin(), max) << endl;
	cout << "Min bin " << distance(fourier.cbegin(), min) << endl;
	cout << "--" << endl;

	// unsigned int iterations = 0;
	for (const auto &f : fourier) {

		static unsigned int bin = 0;

		cout << bin * resolution << "\t" << f.real();

		if (bin == distance(fourier.cbegin(), max))
			cout << "\t<-- MAX";
		else if (bin == distance(fourier.cbegin(), min))
			cout << "\t<-- MIN";

		cout << endl;

		++bin;
	}

	return 0;
}
