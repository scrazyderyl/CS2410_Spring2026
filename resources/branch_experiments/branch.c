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

const unsigned int PROFILE_ITERATIONS = 1000;

size_t arraySize;
bool *array;
bool sorted;
std::vector<int> data;

extern "C" void create() {
    // generate data
    data.resize(arraySize);
    for (unsigned c = 0; c < arraySize; ++c)
        data[c] = std::rand() % 256;

    // optionally sort the data
    if (sorted) {
        std::sort(data.begin(), data.end());
    }

    // generate boolean array
    array = (bool*)malloc(arraySize * sizeof(bool));
    for (unsigned c = 0; c < arraySize; ++c)
        array[c] = (data[c] >= 128);
}

extern "C" void run() {
    // 1. Tell perf to start (write "enable\n" to fd 3)
    write(3, "enable\n", 7);

    for (unsigned i = 0; i < PROFILE_ITERATIONS; i++)
    {
	    long long sum = 0;
	    for (unsigned c = 0; c < arraySize; ++c)
	    {
		// Profile the branch
		if (array[c])
		    sum += data[c];
	    }
    }

    // 2. Tell perf to stop (write "disable\n" to fd 3)
    write(3, "disable\n", 8);
}

int main(int args, char **argv)
{
    if (args != 3) {
        printf("Usage: branch <array size> <sort|nosort>\n");
        return 0;
    }
    arraySize = atoi(argv[1]);
    sorted = strcmp(argv[2], "sort") == 0;

    create();
    run();

    return 0;
}

