#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#include <vector>
#include <numeric>
#include <algorithm>
#include <random>

#define MIN_ACCESSES	10000000
#define DATA_SIZE     2
#define OUTER_LOOP    2

// Define a linked list node type with no data
typedef struct node {
  struct node* next;    // 8 bytes
  int data[DATA_SIZE];  // 8 bytes
} node_t;


// Keep a pointer to the beginning and end of the list
int items = 0;
node_t* head = NULL;

std::vector<int> random_permutation(int length) {
  std::vector<int> arr(length);
  std::iota(arr.begin(), arr.end(), 0);  // fill with 0, 1, 2, ..., length
  std::mt19937 rng(42);  // fixed seed for reproducibility
  std::shuffle(arr.begin(), arr.end(), rng);
  return arr;
}

extern "C" void create() {
  std::vector<int> array = random_permutation(items);

  head = (node_t *) malloc(sizeof(node_t) * items);
  for(int i = 0; i < items; i++) {
    node_t* n = &head[array[i]];
    n->next = (i < items - 1) ? &head[array[i+1]] : NULL;
    for(int j = 0; j < DATA_SIZE; j++) n->data[j] = j;
  }
  head = &head[array[0]];
}

extern "C" void run() {
  // 1. Tell perf to start (write "enable\n" to fd 3)
  if (write(3, "enable\n", 7) != 7) { perror("enable"); exit(1); }

  // Now that we have an array, traverse the array over and over again until we've
  // visited `ACCESSES` elements in our array.
  int accesses = 0, sum = 0;
  while(true) {
    for(int i = 0; i < OUTER_LOOP; i++) {
      for(node_t* current = head; current != NULL; current = current->next) {
        sum += current->data[i];
      }
    }
    accesses += OUTER_LOOP * items;
    if(accesses >= MIN_ACCESSES) break;
  }

  // 2. Tell perf to stop (write "disable\n" to fd 3)
  if (write(3, "disable\n", 8) != 8) { perror("disable"); exit(1); }

  printf("Accesses = %d, Sum = %d\n", accesses, sum);
}

int main(int argc, char** argv) {
  // Check to make sure we received a command line option
  if(argc < 2) {
    fprintf(stderr, "Usage: %s <array size>\n", argv[0]);
    return 1;
  }

  // How many items should we have in the list?
  items = atoi(argv[1]);

  create();
  run();

  return 0;
}
