#include <stdbool.h>

typedef struct Vertex {
	long long int id;
	long long int timestamp;
  long long int *in_edges;
  long long int *out_edges;
  long long int in_degree;
  long long int out_degree;
} Vertex;

typedef struct Edge {
	long long int source_id;
	long long int target_id;
} Edge;

typedef struct Graph {
    long long int vcount;
    Vertex *vs;
    long long int ecount;
} Graph;

#define CREATE_GRAPH(G) Graph G = {.vcount = 0, .ecount = 0}

/* function prototypes for utility.c */
void raise_error(int code);
bool in_int_array(long long int *array, long long int sizeof_array, long long int value);
void add_to_int_array(long long int **array, long long int sizeof_array, long long int value, bool add_memory);

/* function prototypes for graph.c */
bool is_graph_sane(Graph *graph); 
void add_vertex(Graph *graph, long long int id, long long int timestamp);
void add_edge(Graph *graph, long long int source_id, long long int target_id);
void free_graph(Graph *graph);

/* function prototypes for cdindex.c */
double cdindex(Graph *graph, long long int id, long long int time_delta);
double mcdindex(Graph *graph, long long int id, long long int time_delta);
long long int iindex(Graph *graph, long long int id, long long int time_delta);
