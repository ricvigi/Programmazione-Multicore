#ifndef HISTOGRAM_PARALLEL_H
#define HISTOGRAM_PARALLEL_H
void hist_parallel(float *data,
		   int *data_count,
		   float *min_meas,
		   float *max_meas,
		   int *bin_count);
#endif
#ifndef RANDOM_FLOAT_H
#define RANDOM_FLOAT_H
int random_float(int *nElements,
		  const int *datacount,
		  float **array,
		  const float *max);
#endif
#ifndef POPULATE_H
#define POPULATE_H
int populate();
#endif


