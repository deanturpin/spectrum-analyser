#ifndef RIFF_H
#define RIFF_H

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

#endif
