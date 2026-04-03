#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#define ACCESSES 10000000

// Define a linked list node type with no data
typedef struct node {
  struct node* next;    // 8 bytes
} node_t;


// Keep a pointer to the beginning and end of the list
int items = 0;
node_t* head = NULL;

extern "C" void create() {
  head = (node_t *) malloc(sizeof(node_t) * items);
  for(int i=0; i<items-1; i++) {
    node_t* n = &head[i];
    n->next = &head[i+1];
  }
  head[items-1].next = NULL;
}

extern "C" void run() {
  // 1. Tell perf to start (write "enable\n" to fd 3)
  if (write(3, "enable\n", 7) != 7) { perror("enable"); exit(1); }

  // Now that we have an array, traverse the array over and over again until we've
  // visited `ACCESSES` elements in our array.
  node_t* current = head;
  for(int i=0; i<ACCESSES; i++) {
    if(current == NULL) current = head;
    else current = current->next;
  }

  // 2. Tell perf to stop (write "disable\n" to fd 3)
  if (write(3, "disable\n", 8) != 8) { perror("disable"); exit(1); }

  // Just so that the compiler does not remove the loop as redundant code
  printf("Result = %p\n", (void*)current);
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
