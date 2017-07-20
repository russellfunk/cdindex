/* 
  cdindex library.
  Copyright (C) 2017 Russell J. Funk <russellfunk@gmail.com>
   
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "cdindex.h"

/**
 * \function is_graph_sane
 * \brief Run a few basic (not comprehensive) checks on graph data structure.
 *
 * \param graph The input graph.
 *
 * \return Whether the graph is sane.
 */
bool is_graph_sane(Graph *graph) {
  bool sane = true;

  /* vcount should not be negative */
	if (graph->vcount < 0) {
    sane = false;
	}
  else {
    /* vertex ids should be sequential */
    if (graph->vcount > 1) {
			for (long long int i = 1; i < graph->vcount; i++) {
	      if (graph->vs[i-1].id + 1 != graph->vs[i].id) {
					sane = false;
				}
			}
		}
    /* the last vertex id should correspond to the number of vertices - 1 */
		if (graph->vs[graph->vcount - 1].id != graph->vcount - 1) {
			sane = false;
		}
	}
  return sane;
}

/**
 * \function add_vertex
 * \brief Add a vertex to a graph (note the graph must have memory allocated).
 *
 * \param graph The input graph.
 * \param id The new vertex id.
 * \param timestamp The new vertex timestamp.
 */
void add_vertex(Graph *graph, long long int id, long long int timestamp) {

  /* allocate memory for a vertex */
  Vertex *tmp;
  if (graph->vcount == 0) {
    tmp = malloc(sizeof(Vertex));
  }
  else {
    tmp = realloc(graph->vs, (graph->vcount + 1) * sizeof(Vertex));
  }
  if (tmp==NULL) {
    raise_error(0);
  }
  else {
    graph->vs = tmp;
  }

  /* the new vertex id should come at the end of the list */
  if (id != graph->vcount) {
    raise_error(1);
  }
  graph->vs[graph->vcount].id = graph->vcount;
	graph->vs[graph->vcount].timestamp = timestamp;
  graph->vs[graph->vcount].in_degree = 0;
  graph->vs[graph->vcount].out_degree = 0;
  graph->vcount++;
}

/**
 * \function add_edge
 * \brief Add a edge to a graph (note the graph must have memory allocated).
 *
 * \param graph The input graph.
 * \param source_id The source vertex id.
 * \param target_id The target vertex id.
 */
void add_edge(Graph *graph, long long int source_id, long long int target_id) {

  /* confirm vertices are in graph */
  if (source_id >= graph->vcount || target_id >= graph->vcount) {
    raise_error(2);
  }

  /* confirm vertices are in graph */
  /*
  long long int vs_ids[graph->vcount]; 
  for (long long int i = 0; i < graph->vcount; i++) {
     vs_ids[i] = graph->vs[i].id;
  }
  if (!in_int_array(vs_ids, graph->vcount, source_id) || !in_int_array(vs_ids, graph->vcount, target_id)) {
    raise_error(2);
  } 
  */
  
  /* confirm edge is not already in graph */
  else if (in_int_array(graph->vs[source_id].out_edges, graph->vs[source_id].out_degree, target_id)) {
    raise_error(3);
  }
  else {

    /* memory reallocation flags */
    bool reallocate_out_edges = true;
    bool reallocate_in_edges = true;

    /* allocate memory for source_id out_edge record if needed */
    if (graph->vs[source_id].out_degree == 0) {
      graph->vs[source_id].out_edges = malloc(sizeof(long long int));
      reallocate_out_edges = false;
    }

    /* allocate memory for target_id in_edge record if needed */
    if (graph->vs[target_id].in_degree == 0) {
      graph->vs[target_id].in_edges = malloc(sizeof(long long int));
      reallocate_in_edges = false;
    }

    /* check for problems */
    if (graph->vs[target_id].in_edges==NULL || graph->vs[source_id].out_edges==NULL) {
      raise_error(0);
    }

    /* append the new source_id and target_id */
    add_to_int_array(&graph->vs[source_id].out_edges, graph->vs[source_id].out_degree, target_id, reallocate_out_edges);
    add_to_int_array(&graph->vs[target_id].in_edges,  graph->vs[target_id].in_degree, source_id, reallocate_in_edges);

    /* increment degree counts */
    graph->vs[source_id].out_degree++;
    graph->vs[target_id].in_degree++;

    /* increment graph ecount */
	  graph->ecount++;

	}
}

/**
 * \function free_graph
 * \brief Free memory taken by a graph.
 *
 * \param graph The input graph.
 */
void free_graph(Graph *graph) {
  for (long long int i = 0; i < graph->vcount; i++) {
   if (graph->vs[i].in_degree > 0) free(graph->vs[i].in_edges);
   if (graph->vs[i].out_degree > 0) free(graph->vs[i].out_edges);
   }
  free(graph->vs);
}
