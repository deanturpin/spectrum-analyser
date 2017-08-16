Command line Fourier analysis and tone generator. See
[YouTube](https://www.youtube.com/watch?v=hwsOKpBg6zo) for a demo. The code is
written in pure C++ using no third-party libraries (bar the STL :)).

# Example of three tones interacting
![](resources/fourier.gif)

# Build
```bash
$ git clone https://github.com/deanturpin/fourier
Clonage dans 'fourier'...
remote: Counting objects: 130, done.
remote: Compressing objects: 100% (86/86), done.
remote: Total 130 (delta 68), reused 96 (delta 38), pack-reused 0
Réception d'objets: 100% (130/130), 1.34 MiB | 1.87 MiB/s, fait.
Résolution des deltas: 100% (68/68), fait.
Vérification de la connectivité... fait.
$ cd fourier/
$ make analyse
clang++ -Weverything -O3 -Wno-c++98-compat -std=c++14 -o tonegen.o -c tonegen.cpp
clang++   tonegen.o   -o tonegen
clang++ -Weverything -O3 -Wno-c++98-compat -std=c++14 -o fourier.o -c fourier.cpp
clang++   fourier.o   -o fourier
./tonegen | ./fourier | head -40
```

# Tone generator
Takes up to three frequencies and generates a WAV on stdout.
```bash
$ ./tonegen 100 200 300 | aplay
Lecture WAVE 'stdin' : Signed 16 bit Little Endian, Fréquence 8000 Hz, Mono
```

Pipe WAV to Fourier.
```bash
$ ./tonegen 100 200 300 | ./fourier 
```

# See also
* [Fourier transform on Wikipedia](https://en.wikipedia.org/wiki/Fourier_transform#Example)
* [ANSI control codes](http://misc.flogisoft.com/bash/tip_colors_and_formatting)
* [Piano frequencies](https://en.wikipedia.org/wiki/Piano_key_frequencies)
* [Just intonation](https://en.wikipedia.org/wiki/Just_intonation)
