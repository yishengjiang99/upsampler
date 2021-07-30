#define wavetable_size 4096
typedef struct _iterate {
  float* sample;
  int sampleLength;
  int output_sample_block, pos;
  float ratio, frac;
} sampler_iterator;
extern float powf(float base, float exp);
void onval(float f, int p){};
static sampler_iterator iterators[1];
static float output[wavetable_size];

sampler_iterator* init(float* sample, int len) {
  iterators->output_sample_block = wavetable_size;
  iterators->sample = sample;
  iterators->sampleLength = len;
  return iterators;
}
float setRatio(sampler_iterator* iterators, float ratio) {
  iterators->ratio = ratio;
  return iterators->ratio;
}
float hermite4(float frac_pos, float xm1, float x0, float x1, float x2) {
  const float c = (x1 - xm1) * 0.5f;
  const float v = x0 - x1;
  const float w = c + v;
  const float a = w + v + (x2 - x0) * 0.5f;
  const float b_neg = w + a;
  return ((((a * frac_pos) - b_neg) * frac_pos + c) * frac_pos + x0);
}

int upsample_wave_table(sampler_iterator* v, float* outputRef) {
  // v->pos = 0;
  for (int i = 0; i < v->output_sample_block; i++) {
    float fm1 = *(v->sample + v->pos - 1);
    float f1 = *(v->sample + v->pos);
    float f2 = *(v->sample + v->pos + 1);
    float f3 = *(v->sample + v->pos + 2);
    outputRef[i] = v->pos == 0 ? 0 : hermite4(v->frac, fm1, f1, f2, f3);
    onval(v->frac, v->pos);
    v->frac += v->ratio;
    while (v->frac >= 1.0f) {
      v->frac--;
      v->pos++;
    }
    if (v->pos >= v->sampleLength - 8) {
      break;
    }
  }
  if (v->pos < v->sampleLength - 3) {
    return 0;
  } else
    return 1;
}
