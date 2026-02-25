/*
 * Copyright (c) 2026 Wonsun Ahn
 * All rights reserved.
 */

#include <algorithm>
#include <vector>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

const unsigned int PROFILE_ITERATIONS = 100000;
const unsigned int ARRAY_SIZE = 1000;

int modulus = 1;
float *array;
int *indices;

extern "C" void create() {
    // generate data
    array = (float *) malloc(sizeof(float) * ARRAY_SIZE);
    for (unsigned c = 0; c < ARRAY_SIZE; ++c)
        array[c] = 34634634636.22;
    // generate indices for access
    indices = (int *) malloc(sizeof(int) * ARRAY_SIZE);
    for (unsigned c = 0; c < ARRAY_SIZE; ++c)
        indices[c] = c % modulus;
}

extern "C" void run() {
    // 1. Tell perf to start (write "enable\n" to fd 3)
    write(3, "enable\n", 7);

    for (unsigned i = 0; i < PROFILE_ITERATIONS; i++)
    {
	long long sum = 0;
	for (unsigned c = 0; c < ARRAY_SIZE; ++c)
	{
	    	array[indices[c]] /= 32.123123;
	}
    }

    // 2. Tell perf to stop (write "disable\n" to fd 3)
    write(3, "disable\n", 8);
}

int main(int args, char **argv)
{
    if (args != 2) {
        printf("Usage: ipc <modulus>\n");
        return 0;
    }
    modulus = atoi(argv[1]);

    create();
    run();

    return 0;
}

