#include <stdbool.h>

typedef struct Vertex {
	int id;
	long long int timestamp;
  int *in_edges;
  int *out_edges;
  int in_degree;
  int out_degree;
} Vertex;

typedef struct Edge {
	int source_id;
	int target_id;
} Edge;

typedef struct Graph {
    int vcount;
    Vertex *vs;
    int ecount;
} Graph;

#define CREATE_GRAPH(G) Graph G = {.vcount = 0, .ecount = 0}

/* function prototypes for utility.c */
void raise_error(int code);
bool in_int_array(int *array, int sizeof_array, int value);
void add_to_int_array(int **array, int sizeof_array, int value, bool add_memory);

/* function prototypes for graph.c */
bool is_graph_sane(Graph *graph); 
void add_vertex(Graph *graph, int id, int timestamp);
void add_edge(Graph *graph, int source_id, int target_id);
void free_graph(Graph *graph);

/* function prototypes for cdindex.c */
double cdindex(Graph *graph, int id, long long int time_delta);
double mcdindex(Graph *graph, int id, long long int time_delta);
