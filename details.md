# C++ standards
Initially I used the default for clang (C++03) and then bumped the standard as
I used newer features. I moved to C++11 to make use of ```auto``` and
range-based for loops and then C++14 for imaginary constants.

# WAV header
There's a common include file [riff.h](riff.h) to define the WAV header.

# Command line examples
The animated GIF was generated with a bash script.
```bash
# Analyse a series of frequencies
for freq in {200..1000..1}; do ./tonegen $freq | ./fourier | head -48; done

# Play C Major scale
for f in 261.6 293.7 329.6 349.2 392.0 440.0 493.9 523.3; do ./tonegen $f | aplay -q; done
```
