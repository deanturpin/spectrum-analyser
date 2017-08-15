#include "riff.h"
#include <iostream>
#include <limits>
#include <math.h>
#include <vector>

int main(int argc, char *argv[]) {

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
  wav.sample_rate = 8000;
  wav.bit_depth = 16;
  wav.bytes_per_second = wav.sample_rate * wav.bit_depth / 8;
  wav.block_align = 1;
  wav.data_id = 1635017060;
  wav.data_size = 2147483648;

  // Parse command line
  vector<int> frequencies = {

      argc > 1 ? atoi(argv[1]) : 440, argc > 2 ? atoi(argv[2]) : 0,
      argc > 3 ? atoi(argv[3]) : 0};

  // Write the WAV header
  cout.write(reinterpret_cast<char *>(&wav), sizeof(wav));

  // Create a second's worth of samples
  for (unsigned int i = 0; i < wav.sample_rate / 2; ++i) {

    const double phase = 2.0 * M_PI * i;

    // Add all the frequencies into the mix
    unsigned short sample = 0;
    for (const auto &f : frequencies)
      if (f > 0)
        sample += static_cast<unsigned short>(
            sin(phase / (static_cast<double>(wav.sample_rate) / f)) * 0xffff /
            6);

    // 2's comp
    sample = ~sample + 1;

    cout.write(reinterpret_cast<char *>(&sample), sizeof(sample));
  }

  return 0;
}
