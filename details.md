# Files
The main file is [fourier.cpp](fourier.cpp). There's also a tone generator
[tonegen.cpp](tonegen.cpp) and a common include file [riff.h](riff.h) which
defines the WAV header.

```fourier.cpp``` - takes a WAV on std in a prints the Fourier transform as ASCII
art.

```tonegen.cpp``` - takes up to three frequencies in Hertz as parameters and
writes a WAV to stdout. This can be piped to a player such as ```aplay``` or to
```fourier```.

```bash
# Generate major chord and send it to the speaker
$ ./tonegen 440 550 660 | aplay

# Generate chord, analyse it and display the first 100 bins
$ ./tonegen 440 550 660 | ./fourier | head -100
```

# C++ standards
Initially I used the default for clang (C++03) and then bumped the standard as
I used newer features. I moved to C++11 to make use of ```auto``` and
range-based ```for``` loops and then C++14 for imaginary constants.

# Coding standard and linter
I've added a make rule to run ```cppcheck```. And I periodically run
```clang-format``` with the default settings and apply the results. I'm not
sure where this fits in the workflow yet but I like that I don't have to think
about it. And it can be useful to swap in IWYU to weed out any left over
includes. But I don't really like the output so it's used purely as a guide.
```bash
make CC=iwyu
```

# Command line examples
The animated GIF was created with a bash script.
```bash
# Analyse a series of frequencies
for freq in {200..1000..1}; do ./tonegen $freq | ./fourier | head -48; done

# Play C Major scale
for f in 261.6 293.7 329.6 349.2 392.0 440.0 493.9 523.3; do ./tonegen $f | aplay -q; done
```

Screen recording and GIF conversion.
```bash
recordmydesktop
ffmpeg -i out.ogv -loop 0 fourier.gif
```
