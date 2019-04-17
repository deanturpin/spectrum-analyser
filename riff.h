#ifndef RIFF_H
#define RIFF_H

#include "fourier.h"
#include <cassert>
#include <iostream>
#include <vector>

// The WAV format
struct wav_header {

  uint32_t riff_id;
  uint32_t riff_size;
  uint32_t wave_tag;
  uint32_t format_id;
  uint32_t format_size;
  uint16_t format_tag;
  uint16_t channels;
  uint32_t sample_rate;
  uint32_t bytes_per_second;
  uint16_t block_align;
  uint16_t bit_depth;
  uint32_t data_id;
  uint32_t data_size;
};

struct wav_header read_wav_header() {

  assert(sizeof(wav_header) == 44);

  wav_header h;
  std::cin.read(reinterpret_cast<char *>(&h), sizeof h);

  return h;
}

size_t read_samples(std::vector<short> &s) {

  s.clear();
  s.reserve(fourier_bins);
  s.resize(fourier_bins);

  std::cin.read(reinterpret_cast<char *>(s.data()),
                fourier_bins * sizeof(int16_t));

  return s.size();
}

#endif
