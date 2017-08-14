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

	const unsigned int bins = 1470 * 2;
	vector<short> samples(bins);
	cin.read(reinterpret_cast<char*>(samples.data()), bins * sizeof(short));

	// Normalise samples
	vector<double> X(samples.cbegin(), samples.cend());
	for (auto &i : X)
		i /= numeric_limits<short>::max();

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

	// All the bins

	const auto max = *max_element(fourier.cbegin(), fourier.cend(),
			[](const auto &a, const auto &b){ return a.real() < b.real(); });

	const auto min = *min_element(fourier.cbegin(), fourier.cend(),
			[](const auto &a, const auto &b){ return a.real() < b.real(); });

	cout << "Channels " << wav.channels << endl;
	cout << "Sample rate " << wav.sample_rate << " Hz" << endl;
	cout << "Resolution " << wav.sample_rate / bins << " Hz" << endl;
	cout << "Max " << max << endl;
	cout << "--" << endl;

	for (unsigned int i = 40; i < 140; ++i) {

		cout << wav.sample_rate * i / bins << "\t" << fourier.at(i).real();

		if (fourier.at(i).real() == max.real())
			cout << "\t<-- MAX";

		cout << endl;
	}

	/*
	const unsigned int displayBins = 50;
	const unsigned int binFactor = bins / displayBins;

	// Consolidate results into a histogram
	map<unsigned int, double> histogram;
	for (int i = 0; i < N; ++i)
		histogram[i/binFactor] += fourier[i].imag();

	const auto max = *max_element(histogram.cbegin(), histogram.cend(),
			[](const auto &a, const auto &b){ return a.second < b.second; });

	const auto min = *min_element(histogram.cbegin(), histogram.cend(),
			[](const auto &a, const auto &b){ return a.second < b.second; });

	cout << "Channels " << wav.channels << endl;
	cout << "Sample rate " << wav.sample_rate << " Hz" << endl;
	cout << "Resolution " << wav.sample_rate / bins << " Hz" << endl;
	cout << "--" << endl;

	// Normalise histogram
	unsigned int iteration = 0;
	for (auto i = histogram.cbegin(); i != histogram.cend(); ++i) {

		cout << wav.sample_rate * i->first / histogram.size() << "\t| " << i->second;

		if (i->first == max.first)
			cout << "\t<-- MAX";
		else if (i->first == min.first)
			cout << "\t<-- MIN";

		cout << endl;
	}
	*/

	return 0;
}
