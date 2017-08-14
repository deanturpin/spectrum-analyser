Command line Fourier analysis and tone generator.

See [YouTube](https://www.youtube.com/watch?v=hwsOKpBg6zo) for a demo.

# Run things when they're rebuilt
```bash
while :; do inotifywait -qe modify tonegen; sleep .1; ./tonegen 27 | aplay; done
```

# Generate tones and write them to WAV
```bash
for freq in {100..22050..30}; do echo Freq $freq Hz; ./tonegen $freq > tone.wav; aplay -q tone.wav; sleep .1; done
```

# Analyse WAV when it changes
```bash
while :; do inotifywait -q -e modify tone.wav >& /dev/null; ./fourier < tone.wav | head -58; done
```

# Generate some tones
## Middle C
```
for f in 261.6 277.2 293.7 311.1 329.6 349.2 370.0 392.0 415.3 440.0 466.2 493.9; do ./tonegen $f | aplay -q; done
```

## C Major

```bash
for f in 261.6 293.7 329.6 349.2 392.0 440.0 493.9 523.3; do ./tonegen $f | aplay -q; done
```

# Pretty printing
* http://misc.flogisoft.com/bash/tip_colors_and_formatting
* https://en.wikipedia.org/wiki/Fourier_transform#Example
