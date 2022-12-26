#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define FS    44100.0f

#define BIRDS 4
#define DIRTY_REVERB_ENABLED

#ifdef  DIRTY_REVERB_ENABLED
#define DIRTY_REVERB_SAMPLES 8000
#define DIRTY_REVERB_DECAY   0.25f
#endif

#define T2SAMP(time)    (time * FS)
#define SAMP2T(samples) (samples / FS)

#define RAD (2.0f * M_PI)

float dirty_reverb(float sample) {
static float buf[DIRTY_REVERB_SAMPLES];
static int   buf_idx;

	buf[buf_idx] = buf[buf_idx] * DIRTY_REVERB_DECAY + sample * (1.0f - DIRTY_REVERB_DECAY);
	float r = buf[buf_idx];
	if (++buf_idx > DIRTY_REVERB_SAMPLES)
		buf_idx = 0;
	return r;
}

float randf(float scale, float offset) {
	union {
		float    f;
		uint32_t ui;
	} r;
	r.ui = 0x3F800000 |
	       (rand() >> 3) |
	       ((rand() & 2047) << 12);
	return (r.f - 1.f) * scale + offset;
}

typedef struct {
	int   state;
	float vol;
	float f_avg, f_delta;
	float f_vib;
	float on_mag, on_base;
	float off_mag, off_base;
	float vib_mag, vib_base;
} bird_t;

static bird_t voices[BIRDS];

float get_bird(float t, int voice_idx) {
	float f_avg   = voices[voice_idx].f_avg;
	float f_delta = voices[voice_idx].f_delta;
	float f_vib   = voices[voice_idx].f_vib;
	float vol     = voices[voice_idx].vol;
	return vol * sinf(RAD * (f_avg * t - f_delta * cosf(RAD * f_vib * t) / (RAD * f_vib)));
}

int main() {

	srand(time(NULL));
	for (int idx = 0; idx < BIRDS; idx++) {
		voices[idx].state    = 0;
		voices[idx].vol      = .5f * powf(1.f / (idx + 1.f), 1.6f);
		voices[idx].f_avg    = 3000.f - idx * 250.f;
		voices[idx].f_delta  = 750.f;
		voices[idx].f_vib    = 20.f;
		voices[idx].on_mag   = 8000.f + idx * 5000.f;
		voices[idx].on_base  = 5000.f;
		voices[idx].off_mag  = 20000.f + idx * 10000.f;
		voices[idx].off_base = 8000.f * BIRDS;
		voices[idx].vib_mag  = 10.f;
		voices[idx].vib_base = 8.f;
	}

	for (int samples = 0;; samples++) {
		float sample = 0.f;
		float t = SAMP2T(samples);

		for (int idx = 0; idx < BIRDS; idx++) {
			sample += voices[idx].state > 0 ? get_bird(t, idx) : 0.f;
			if (abs(voices[idx].state) <= 1) {
				voices[idx].state = voices[idx].state > 0
					? - randf(voices[idx].off_mag, voices[idx].off_base)
					: randf(voices[idx].on_mag, voices[idx].on_base);
				voices[idx].f_vib = randf(voices[idx].vib_mag, voices[idx].vib_base);
			} else
				voices[idx].state = voices[idx].state + (voices[idx].state > 1 ? -1 : +1);
		}
		sample     = sample > 1.f ? 1.f : sample;
		sample     = sample < -1.f ? -1.f : sample;
#ifdef DIRTY_REVERB_ENABLED
		sample     = dirty_reverb(sample);
#endif
		int16_t sample_s16 = sample * 32767.f;
		putchar(sample_s16 & 0xFF);
		putchar(sample_s16 >> 8);
	}
}
