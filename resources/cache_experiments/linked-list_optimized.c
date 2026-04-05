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
#define OUTER_LOOP	4

// Define a linked list node type with no data
typedef struct node {
  struct node* next;    // 8 bytes
  int data[14];         // 56 bytes
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
  for(int i=0; i<items-1; i++) {
    node_t* n = &head[array[i]];
    n->next = &head[array[i+1]];
  }
  head[array[items-1]].next = NULL;
  head = &head[array[0]];
}

extern "C" void run() {
  // 1. Tell perf to start (write "enable\n" to fd 3)
  if (write(3, "enable\n", 7) != 7) { perror("enable"); exit(1); }

  // Now that we have an array, traverse the array over and over again until we've
  // visited `ACCESSES` elements in our array.
  volatile int accesses = 0;
  while(true) {
    for(node_t* current = head; current != NULL; current = current->next) {
      #pragma GCC unroll 1  // Pragma prevents unrolling of inner loop, for
                            // an apples-to-apples comparison.
      for(int i = 0; i < OUTER_LOOP; i++) {
        accesses++;
      }
    }
    if(accesses >= MIN_ACCESSES) break;
  }

  // 2. Tell perf to stop (write "disable\n" to fd 3)
  if (write(3, "disable\n", 8) != 8) { perror("disable"); exit(1); }

  // Just so that the compiler does not remove the loop as redundant code
  printf("Accesses = %d\n", accesses);
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
