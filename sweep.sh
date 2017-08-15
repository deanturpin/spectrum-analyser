#!/bin/bash

readonly rate=8000

for freq in 261.6 293.7 329.6 349.2 392.0 440.0 493.9 523.3; do

  # Play the sound
  ./tonegen $freq $rate | aplay --quiet

  # Analyse it
  ./tonegen $freq $rate | ./fourier | head -80

  sleep .75
done
