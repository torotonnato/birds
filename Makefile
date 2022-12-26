all: birds lonely_obfuscated_bird

birds: birds.c
	gcc -O2 birds.c -o birds -lm

lonely_obfuscated_bird: lonely_obfuscated_bird.c
	gcc lonely_obfuscated_bird.c -o lonely_obfuscated_bird -lm

clean:
	rm birds lonely_obfuscated_bird
