
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "hls_config.h"

#define REL_TOL 1
#define TRIALS 5

int main(void)
{
	printf("Running C simulation!");

	int testResult = 0;
	FILE *fdata;
	data_t output[2];
	data_t x[N];
	data_t buffer[2];
	float add_rel_err;
	float mn_rel_err;

	fdata =fopen("goldenReference.dat","r");

	if (fdata == NULL)
	{
		printf("Error reading goldenReference file!\n");
		exit(1);
	}

	for (int i = 0; i < TRIALS; i++)
	{
		/* Read vector */
		for (int j = 0; j < N; ++j)
		{
			fscanf(fdata, "%f\n", &x[j]);
		}
		/* Read reference results */
		fscanf(fdata, "%f\n", &buffer[0]);
		fscanf(fdata, "%f\n", &buffer[1]);
		/* Call hls function */
		hls_example(&output[0], &output[1], x);

		/* Compute error */
		add_rel_err = 0;
		mn_rel_err = 0;

		add_rel_err = 100 * fabs((output[0] - buffer[0])/buffer[0]);
		mn_rel_err = 100 * fabs((output[1] - buffer[1])/buffer[1]);
		printf("TRIAL: %d,\t Expected: [%.3f,%.3f]\tGot:[%.3f,%.3f] Err: [%.6f%%,%.6f%%]\n", i,buffer[0], buffer[1],output[0],output[1],add_rel_err, mn_rel_err);
		if (add_rel_err > REL_TOL)
		{
			printf("Sum error exceeds tolerance!\n");
			testResult += 1;
		}
		if (mn_rel_err > REL_TOL)
		{
			printf("Mean error exceeds tolerance!\n");
			testResult += 1;
		}

	}

	fclose(fdata);

	if (testResult)
	{
		printf("*******************************************\n");
		printf("*\t \t  FAIL \n");
		printf("*******************************************\n");
	}
	else
	{
		fprintf(stdout, "*******************************************\n");
		fprintf(stdout, "*\t \t  PASS \n");
		fprintf(stdout, "*******************************************\n");
	}
	return testResult;

}
