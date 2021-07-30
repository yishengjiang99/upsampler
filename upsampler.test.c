#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "upsample.c"
int main() {
  float output[4096];
  bzero(output, 4096 * 4);
  float sample[49000];
  for (int i = 0; i < 49000; i++) {
    sample[i] = sinf((float)(2 * 3.1415926 * i * 440));
  }
  sampler_iterator* it = init(sample, 49000);

  it->sample = sample;
  setRatio(it, 5.0f);
  upsample_wave_table(it, output);
  printf("rat %f", it->ratio);
  for (int i = 0; i < it->output_sample_block; i++) {
    printf("\n%d, %f", i, output[i]);
  }
}