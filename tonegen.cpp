#include <math.h>
#include <limits>
#include <iostream>

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
		unsigned int tbd : 16;
		unsigned int block_align : 16;
		unsigned int data_id;
		unsigned int data_size;
	};
}

int main(int argc, char* argv[]) {

	using namespace std;

	// Populate the WAV header
	riff::wav wav;

	wav.riff_id = 1179011410;
	wav.riff_size = 2147483684;
	wav.wave_tag = 1163280727;
	wav.format_id = 544501094;
	wav.format_size = 16;
	wav.format_tag = 1;
	wav.channels = 1;
	wav.sample_rate = 44100;
	wav.bytes_per_second = 44100;
	wav.tbd = 0x1;
	wav.block_align = 0x8;
	wav.data_id = 1635017060;
	wav.data_size = 2147483648;

	cout.write(reinterpret_cast<char *>(&wav), sizeof(wav));

	const unsigned int samplesPerCycle = (argc > 1 ? atoi(argv[1]) : 100);
	const int amplitude = 10;

	for (unsigned int i = 0; i < samplesPerCycle * 2000; ++i) {

		const double phase = i * 2 * M_PI / samplesPerCycle;
		short sample = numeric_limits<short>::max() + round(amplitude * sin(phase));

		cout.write(reinterpret_cast<char *>(&sample), sizeof(sample));
		// cerr << sample << endl;
	}

	return 0;
}
