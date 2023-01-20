#include <assert.h>
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "lib/bird_audio.h"

//Number of voices
#define BIRDS 4

#define DIRTY_REVERB_ENABLED
#ifdef  DIRTY_REVERB_ENABLED
#define DIRTY_REVERB_DECAY   0.25f
#endif

//Defines an open interval [min, min+len)
typedef struct {
	float min, len;
} interval_t;

typedef struct {
	int            state;
	fm_synth_arg_t fm_descr;
	interval_t     on_time, off_time, vib;
} bird_t;


bird_t *build_birds(int birds_num);
void   evolve_bird_state(bird_t *bird);

int main()
{
	bird_t *voices = build_birds(BIRDS);
	srand(time(NULL));

	for (int samples = 0;; samples++) {
		float sample = 0.f;
		float t = sample_to_time(samples);

		for (int idx = 0; idx < BIRDS; idx++) {
			evolve_bird_state(&voices[idx]);
			if (voices[idx].state > 0)
				sample += fm_synth(t, &voices[idx].fm_descr);
		}
#ifdef DIRTY_REVERB_ENABLED
		sample = dirty_reverb_effect(hard_clip(sample), DIRTY_REVERB_DECAY);
#endif
		//Quantize the sample to 16 bits (signed)
		int16_t sample_s16 = hard_clip(sample) * 32767.f;
		putchar(sample_s16 & 0xFF);
		putchar(sample_s16 >> 8);
	}
}

bird_t *build_birds(int birds_num)
{
	bird_t *voices = (bird_t*)malloc(sizeof(bird_t) * birds_num);
	assert(voices);

	for (int idx = 0; idx < birds_num; idx++) {
		voices[idx].state            = 0;
		voices[idx].fm_descr.vol     = .5f * powf(1.f / (idx + 1.f), 1.6f);
		voices[idx].fm_descr.f_avg   = 3000.f - idx * 250.f;
		voices[idx].fm_descr.f_delta = 750.f;
		voices[idx].fm_descr.f_vib   = 20.f;
		voices[idx].on_time.len      = 8000.f + idx * 5000.f;
		voices[idx].on_time.min      = 5000.f;
		voices[idx].off_time.len     = 20000.f + idx * 10000.f;
		voices[idx].off_time.min     = 8000.f * birds_num;
		voices[idx].vib.len          = 10.f;
		voices[idx].vib.min          = 8.f;
	}

	return voices;
}

//Returns a random number inside the open interval i
float rand_in_interval(interval_t *i)
{
	return drand48() * i->len + i->min;
}

void evolve_bird_state(bird_t *bird)
{
	//If bird is ready to switch state...
	if (abs(bird->state) <= 1) {
		if (bird->state > 0)
			//If voice is on, prepare for pause using off_time
			bird->state = -rand_in_interval(&bird->off_time);
		else
			//If voice is off, prepare for activation using on_time
			bird->state = rand_in_interval(&bird->on_time);
		//Regenerate the vibrato parameter
		bird->fm_descr.f_vib = rand_in_interval(&bird->vib);
	} else
		bird->state += bird->state > 0 ? -1 : +1;
}
