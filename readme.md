Command line Fourier analysis and tone generator. See
[YouTube](https://www.youtube.com/watch?v=hwsOKpBg6zo) for a demo. The code is
written in pure C++ using no third-party libraries (bar the STL :)). 

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

# Example of three tones interacting
![](resources/fourier.gif)

# Tone generator
Takes up to three frequencies
```bash
$ ./tonegen 100 200 300 | aplay
Lecture WAVE 'stdin' : Signed 16 bit Little Endian, Fréquence 8000 Hz, Mono
```

Pipe to Fourier
```bash
$ ./tonegen 100 200 300 | ./fourier 
```

# C++ standards
* 11 - ranger-based loops
* 14 - extended complex number support

# Resources
* [Fourier transform on Wikipedia](https://en.wikipedia.org/wiki/Fourier_transform#Example)
* [ANSI control codes](http://misc.flogisoft.com/bash/tip_colors_and_formatting)
