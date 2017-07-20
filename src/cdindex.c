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
#include <stdbool.h>
#include "cdindex.h"

/**
 * \function cdindex
 * \brief Computes the CD Index.
 *
 * \param graph The input graph.
 * \param id The focal vertex id.
 * \param time_delta Time beyond stamp of focal vertex to consider in measure.
 *
 * \return The value of the CD index.
 */
double cdindex(Graph *graph, long long int id, long long int time_delta){

   /* Build a list of "it" vertices that are "in_edges" of the focal vertex's
     "out_edges" as of timestamp t. Vertices in the list are unique. */

   long long int it_count = 0;
   long long int *it = malloc(sizeof(long long int));

   /* check for malloc problems */
   if (it==NULL) {
     raise_error(0);
   }

   /* define i for multiple loops */
   long long int i;

   /* add unique "in_edges" of focal vertex "out_edges" */
   for (i = 0; i < graph->vs[id].out_degree; i++) {
     long long int out_edge_i = graph->vs[id].out_edges[i];
     for (long long int j = 0; j < graph->vs[out_edge_i].in_degree; j++) {
       long long int out_edge_i_in_edge_j = graph->vs[out_edge_i].in_edges[j];
       if (graph->vs[out_edge_i_in_edge_j].timestamp > graph->vs[id].timestamp &&
           graph->vs[out_edge_i_in_edge_j].timestamp <= (graph->vs[id].timestamp + time_delta) &&
           !in_int_array(it, it_count, out_edge_i_in_edge_j)) {
         add_to_int_array(&it, it_count, out_edge_i_in_edge_j, true);
         it_count++;
       }
      }
     }

   /* add unique "in_edges" of focal vertex */
   for (i = 0; i < graph->vs[id].in_degree; i++) {
     long long int in_edge_i = graph->vs[id].in_edges[i];
     if (graph->vs[in_edge_i].timestamp > graph->vs[id].timestamp &&
         graph->vs[in_edge_i].timestamp <= (graph->vs[id].timestamp + time_delta) &&
         !in_int_array(it, it_count, in_edge_i)) {
       add_to_int_array(&it, it_count, in_edge_i, true);
       it_count++;
       }
     }

  /* compute the cd index */
  double sum_i = 0.0;
  for (i = 0; i < it_count; i++) {
    long long int f_it = in_int_array(graph->vs[it[i]].out_edges, graph->vs[it[i]].out_degree, id);
    long long int b_it = 0;
    for (long long int j = 0; j < graph->vs[it[i]].out_degree; j++) {
      if (in_int_array(graph->vs[id].out_edges, graph->vs[id].out_degree, graph->vs[it[i]].out_edges[j])) {
        b_it = 1;
      }
    }
    sum_i += -2.0*f_it*b_it + f_it;
  }

  free(it);
  return sum_i/it_count;
}

/**
 * \function iindex
 * \brief Computes the I Index (i.e., the in degree of the focal vertex at time t).
 *
 * \param graph The input graph.
 * \param id The focal vertex id.
 * \param time_delta Time beyond stamp of focal vertex to consider in computing the measure.
 *
 * \return The value of the I index.
 */
long long int iindex(Graph *graph, long long int id, long long int time_delta){

   /* count mt vertices that are "in_edges" of the focal vertex as of timestamp t. */
   long long int mt_count = 0;
   for (long long int i = 0; i < graph->vs[id].in_degree; i++) {
     long long int in_edge_i = graph->vs[id].in_edges[i];
     if (graph->vs[in_edge_i].timestamp <= (graph->vs[id].timestamp + time_delta)) {
       mt_count++;
       }
     }

  return mt_count;
}

/**
 * \function mcdindex
 * \brief Computes the mCD Index.
 *
 * \param graph The input graph.
 * \param id The focal vertex id.
 * \param time_delta Time beyond stamp of focal vertex to consider in computing the measure.
 *
 * \return The value of the mCD index.
 */
double mcdindex(Graph *graph, long long int id, long long int time_delta){

  double cdindex_value = cdindex(graph, id, time_delta);
  long long int iindex_value = iindex(graph, id, time_delta);

  return cdindex_value * iindex_value;

}
