#include "riff.h"
#include <math.h>
#include <limits>
#include <iostream>

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

	// Parse command line
	const unsigned int frequency
		= (argc > 1 ? static_cast<unsigned int>(atoi(argv[1])) : 440);

	// Calculate samples required for a full cycle at the target frequency
	const unsigned int samplesPerCycle = wav.sample_rate / frequency;

	// Write the WAV header
	cout.write(reinterpret_cast<char *>(&wav), sizeof(wav));

	// Write the samples
	for (unsigned int i = 0; i < 1470*4; ++i) {

		const double phase = 2.0 * M_PI * i/samplesPerCycle;
		auto sample = static_cast<unsigned short>(sin(phase) * 0xffff/2);

		sample = ~sample + 1;

		cout.write(reinterpret_cast<char *>(&sample), sizeof(sample));
	}

	return 0;
}
