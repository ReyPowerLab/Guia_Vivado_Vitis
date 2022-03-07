
#ifndef HLS_CONFIG

#define HLS_CONFIG

/* Vector length */
#define N 128

/* Data type */
typedef float data_t;

void hls_example(data_t *y_add, data_t *y_mean, data_t x[N]);

#endif
