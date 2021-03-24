/**
 * Document: MaxCompiler Tutorial (maxcompiler-tutorial.pdf)
 * Chapter: 6      Example: 3      Name: Vectors
 * MaxFile name: Vectors
 * Summary:
 *    Streams a vector of integers to the dataflow engine and confirms that the
 *    returned stream contains the integers values doubled.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Maxfiles.h"
#include <MaxSLiCInterface.h>

int check(int vectorSize, uint32_t *outVector, uint32_t *expectedVector)
{
	int status = 0;
	for (int i = 0; i < vectorSize * Vectors_vectorSize; i++) {
		if (outVector[i] != expectedVector[i]) {
			fprintf(stderr, "Error detected outVector[%d] != %d\n",
				outVector[i], expectedVector[i]);
			status = 1;
		}
	}
	return status;
}

void VectorsCPU(int32_t vectorSize, uint32_t *inVector1, uint32_t *inVector2, uint32_t *outVector)
{
	/*for (int i = 0; i < vectorSize * Vectors_vectorSize; i++) {
		outVector[i] = inVector1[i] * 8;
	}*/
	
	for (int i = 0; i < sqrt(vectorSize); i++){
		for (int j = 0; j < sqrt(vectorSize); j++){
    	    int temp = 0;
		    for (int k = 0; k < sqrt(vectorSize); k++){
		        temp += inVector1[i*(int)sqrt(vectorSize) + k] * inVector2[k*(int)sqrt(vectorSize)+j];
		    }
		    outVector[i*(int)sqrt(vectorSize)+j] = temp;
		}
	}
}

int main()
{
	const int vectorSize = Vectors_vectorSize;
	const int streamSize = 16;
	size_t sizeBytes = vectorSize * streamSize * sizeof(uint32_t);
	uint32_t *inVector1 = malloc(sizeBytes);
	uint32_t *inVector2 = malloc(sizeBytes);
	uint32_t *outVector = malloc(sizeBytes);
	uint32_t *expectedVector = malloc(sizeBytes);

	for (int i = 0; i < vectorSize * streamSize; i++) {
		inVector1[i] = i;
		inVector2[i] = i;
	}

	for (int i = 0; i < vectorSize * streamSize; i++) {
		outVector[i] = 0;
	}

	VectorsCPU(streamSize, inVector1, inVector2, outVector);

	printf("Running DFE.\n");
	Vectors(streamSize, inVector1, inVector2, expectedVector);

	int status = check(streamSize, outVector, expectedVector);
	if (status)
		printf("Test failed.\n");
	else
		printf("Test passed OK!\n");

	return status;
}
