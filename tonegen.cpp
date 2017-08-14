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
		unsigned int block_align : 16;
		unsigned int bit_depth : 16;
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
	wav.bit_depth = 8;
	wav.block_align = 1;
	wav.data_id = 1635017060;
	wav.data_size = 2147483648;

	cout.write(reinterpret_cast<char *>(&wav), sizeof(wav));

	const unsigned int frequency
		= (argc > 1 ? static_cast<unsigned int>(atoi(argv[1])) : 440);

	const unsigned int samplesPerCycle = wav.sample_rate / frequency;

	for (unsigned int i = 0; i < 1470*4; ++i) {

		const double phase = 2.0 * M_PI * i/samplesPerCycle;
		unsigned short sample = static_cast<short>(sin(phase) * 0xffff/2);

		sample = ~sample + 1;

		cout.write(reinterpret_cast<char *>(&sample), sizeof(sample));

		// cerr << sample << endl;
	}

	return 0;
}
