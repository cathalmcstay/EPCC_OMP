/**
 * @file asynchronous.c
 * @brief This example is to illustrate how to leverage asynchronous execution.
 * @details You will find two folders in this example, and will in all examples
 * provided in this session:
 * - src: contains the source code.
 * - bin: contains the binary produced.
 *
 * The makefile provided already sets everything up for you:
 * - To compile: `make`.
 * - To execute: `./bin/asynchronous`.
 *
 * If you have any questions, do not hesitate.
 * @author Ludovic Capelli (l.capelli@epcc.ed.ac.uk)
 **/

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

/**
 * @brief In this exercise, you must issue each of the four assignments in a
 * different target construct, and take care of underlying dependencies. You
 * must also execute your offloading in an asynchronous way.
 **/
int main(int argc, char* argv[])
{
	int a;
	int b;
	int c;
	int d;

	#pragma omp target enter data map(alloc:a,b,c,d)
	// Offload this assignment
	#pragma omp target nowait map(from:a) depend(out:a)
	{
		a = 123;
	}
	// Offload this assignment
	#pragma omp target nowait map(from:b) depend(out:b)
	{
		b = 123;
	}
	// Offload this assignment
	#pragma omp target nowait map(to:a, b) map(from:c) depend(in:a, b) depend(out:c)
	{
		c = a + b;
	}
	// Offload this assignment
	#pragma omp target nowait map(to:a, c) map(from:d) depend(in:a, c) depend(out:d)
	{
		d = a + c;
	}
	#pragma omp taskwait
	#pragma omp target update from(a,b,c,d)
	#pragma omp target exit data map(delete:a,b,c,d)
	printf("a = %d.\n", a);
	printf("b = %d.\n", b);
	printf("c = %d.\n", c);
	printf("d = %d.\n", d);

	return EXIT_SUCCESS;
}
