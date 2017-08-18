Command line DFT and tone generator. See live demo on
[YouTube](https://www.youtube.com/watch?v=hwsOKpBg6zo). The code is written in
C++ and the STL.

See the [details](details.md).

# Example of three tones interacting
![](fourier.gif)

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
$ make demo
clang++ -Weverything -O3 -Wno-c++98-compat -std=c++14 -o tonegen.o -c tonegen.cpp
clang++   tonegen.o   -o tonegen
clang++ -Weverything -O3 -Wno-c++98-compat -std=c++14 -o fourier.o -c fourier.cpp
clang++   fourier.o   -o fourier
./tonegen | ./fourier | head -40
```
