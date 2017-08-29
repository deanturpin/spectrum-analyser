#ifndef RIFF_H
#define RIFF_H

#include <vector>
#include <iostream>

// The WAV format
namespace rif {

struct _header {

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
} header;

std::vector<short> read_samples(const unsigned int);
std::vector<short> read_samples(const unsigned int size) {

  // Read the WAV header
  std::cin.read(reinterpret_cast<char *>(&header), sizeof header);

  // Read a batch of samples
  std::vector<short> samples(size);
  std::cin.read(reinterpret_cast<char *>(samples.data()), size * sizeof(short));

  // Convert to decimal
  for (auto &i : samples)
    i = ~(i - 1);

  return samples;
}
}

#endif
