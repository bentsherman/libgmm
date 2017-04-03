#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "gmm.h"
#include "seqGmm.h"
#include "parallelGmm.h"
#include "util.h"

int main(int argc, char** argv) {
	srand(time(NULL));

	const size_t minPointDim = 1;
	const size_t maxPointDim = 64;

	const size_t numPoints = 4096;
	const size_t numComponents = 8;

	const size_t numSamples = 1;

	struct timeval start, end;

	fprintf(stdout, "#numPoints numComponents pointDim seqElapsedSec parallelElapsedSec\n");
	for(size_t sample = 0; sample < numSamples; ++sample) {	
		for(size_t pointDim = minPointDim; pointDim < maxPointDim; pointDim *= 2) {
			double* X = generateGmmData(numPoints, pointDim, numComponents);

			gettimeofday(&start, NULL);
			freeGMM(fit(X, numPoints, pointDim, numComponents));
			gettimeofday(&end, NULL);
			double seqElapsedSec = calcElapsedSec(&start, &end);


			gettimeofday(&start, NULL);
			freeGMM(parallelFit(X, numPoints, pointDim, numComponents));
			gettimeofday(&end, NULL);
			double parallelElapsedSec = calcElapsedSec(&start, &end);

			fprintf(stdout, "%zu %zu %zu %f %f\n", 
				numPoints, numComponents, pointDim, seqElapsedSec, parallelElapsedSec);

			free(X);
		}
	}

	return EXIT_SUCCESS;
}
