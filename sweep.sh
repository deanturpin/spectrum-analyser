#!/bin/bash

for freq in 261.6 293.7 329.6 349.2 392.0 440.0 493.9 523.3; do

	# Play the sound
	./tonegen $freq | aplay --quiet &

	# Analyse it
	./tonegen $freq | ./fourier | head -48;

	sleep 5
done
