#ifndef BIRD_AUDIO_H
#define BIRD_AUDIO_H

#define SAMPLE_RATE          44100.0f
#define DIRTY_REVERB_SAMPLES 8000

typedef struct {
	float vol, f_avg, f_delta, f_vib;
} fm_synth_arg_t;

//Has global context
float dirty_reverb_effect(float sample, float decay);
float fm_synth(float t, fm_synth_arg_t *arg);
float hard_clip(float sample);
float sample_to_time(int sample_num);

#endif
