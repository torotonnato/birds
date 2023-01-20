# birds

Now with more chirping!

A simple experiment in FM audio synthesis.

Outputs a mono audio stream @44.1KHz, (signed) 16 bits per sample.


````
make; build/birds | aplay -f S16_LE -r 44100 -c 1
````


Bonus: obfuscated single voice version with LOTS of warnings. Output @22050Hz

```
make; build/obfuscated_lonely_bird | aplay -f S16_LE -r 22050 -c 1
```

[Sample](sample/birds.mp3)


![Sample FFT](sample/birds_fft.png)
