all: build/birds build/obfuscated_lonely_bird

build/birds: birds.c
	gcc -O2 birds.c lib/bird_audio.c -o build/birds -lm

build/obfuscated_lonely_bird: obfuscated_lonely_bird.c
	@echo "\e[31m"
	@echo "********************************************"
	@echo "******* WARNING: LOTS OF WARNINGS **********"
	@echo "********************************************"
	@echo "\e[0m"
	gcc obfuscated_lonely_bird.c -o build/obfuscated_lonely_bird -lm -Wno-implicit-int -Wno-implicit-function-declaration

clean:
	rm build/birds build/obfuscated_lonely_bird
