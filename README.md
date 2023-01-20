# birds

Now with more chirping!

A simple experiment in FM audio synthesis.

Outputs a mono audio stream @44.1KHz, (signed) 16 bits per sample.


````
make; build/birds | aplay -f S16_LE -r 44100 -c 1
````


Bonus: obfuscated single voice version with LOTS of warnings.

[Sample](sample/birds.mp3)


![Sample FFT](sample/birds_fft.png)
