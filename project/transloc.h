#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>

#define CR 13
#define LF 10
#define SP 32
#define NUM_THREADS 100

typedef struct subgene_t {
   int name; // tail:0; head:1;
   int done; // 0: not done; 1: done;
   int chromoid;
   struct gene_t *parent;
   struct subgene_t *gray;  // gray edge;
} subgene;

typedef struct gene_t {
   int name;
   subgene left;
   subgene right;
   struct gene_t *prev; // black edge;
   struct gene_t *next; // black edge;
} gene;

typedef struct chromosome_t {
   int name;
   gene *g;
   struct chromosome_t *prev;
   struct chromosome_t *next;  
} chromosome;

typedef struct genome_t {
   int name;
   int nchromo;
   int ngene;
   chromosome *c;
} genome;

typedef struct cycle_t {
   gene *g;
   struct cycle_t *next;
} cycle;

/* Global variables */
genome *genome1_head, *genome2_head;
cycle *pcycle_head, *pcycle_tail; // a circular linked list;
pthread_mutex_t pcycle_head_lock, pcycle_tail_lock;
pthread_mutex_t g1cycle_lock, g2cycle_lock;
 int pcycle, g1cycle, g2cycle;

