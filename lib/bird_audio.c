#include "bird_audio.h"
#include <stdlib.h>
#include <math.h>

static const float TAU          = 6.2831853f;
static const float RSAMPLE_RATE = 1.0f / SAMPLE_RATE;

//Has global context
float dirty_reverb_effect(float sample, float decay)
{
static float buf[DIRTY_REVERB_SAMPLES];
static int   buf_idx;

        float ret = buf[buf_idx] * decay + sample * (1.0f - decay);

        buf[buf_idx++] = ret;
        if (buf_idx >= DIRTY_REVERB_SAMPLES)
                buf_idx = 0;

        return ret;
}

float fm_synth(float t, fm_synth_arg_t *arg)
{
	float mod = arg->f_delta * cosf(TAU * arg->f_vib * t) / arg->f_vib;
	return arg->vol * sinf(TAU * arg->f_avg * t - mod);
}

static float maxf(float a, float b)
{
	return a > b ? a : b;
}

static float minf(float a, float b)
{
	return a < b ? a : b;
}

float hard_clip(float sample) 
{
	return maxf(minf(sample, 1.0f), -1.0f);
}

float sample_to_time(int sample_num)
{
	return sample_num * RSAMPLE_RATE;
}
