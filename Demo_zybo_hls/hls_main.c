
#include "hls_config.h"

void hls_example(data_t *y_add, data_t *y_mean, data_t x[N])
{
#pragma HLS INTERFACE mode=s_axilite port=x storage_impl=bram
#pragma HLS INTERFACE mode=s_axilite port=y_add
#pragma HLS INTERFACE mode=s_axilite port=y_mean
#pragma HLS INTERFACE mode=s_axilite port=return
#pragma HLS ARRAY_PARTITION variable=x type=cyclic factor=8

	data_t res = 0;

	MainLoop: for (int i = 0; i < N; ++i)
	{
#pragma HLS UNROLL
		res = res + x[i];
	}
	*y_add = res;
	*y_mean = res/N;
}
