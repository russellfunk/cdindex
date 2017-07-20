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

int main() {

  /* initialize graph components */
  int VERTICES = 11;
  int EDGES = 13;

  /* dummy data */
  long long int raw_vertices_time[] = {694245600,
                                       694245600,
                                       725868000,
                                       725868000,
                                       788940000,
                                       852098400,
                                       883634400,
                                       915170400,
                                       915170400,
                                       883634400,
                                       852098400};
  long long int raw_edges[13][2] = {{4,2},
                          {4,0},
                          {4,1},
                          {4,3},
                          {5,2},
                          {6,2},
                          {6,4},
                          {7,4},
                          {8,4},
                          {9,4},
                          {9,1},
                          {9,3},
                          {10,4}};

  /* create an empty graph */
  CREATE_GRAPH(g);

  /* add vertices to the graph */
  for (int i = 0; i < VERTICES; i++) {
    add_vertex(&g, i, raw_vertices_time[i]);
  }

  /* add edges to the graph */
  for (int p = 0; p < EDGES; p++) {
    add_edge(&g, raw_edges[p][0], raw_edges[p][1]);
  }

  /* test and report sanity */
    printf("Testing graph sanity: %s\n", is_graph_sane(&g) ? "PASS" : "FAIL");

  /* compute cdindex measure */
  printf("CD index: %f\n", cdindex(&g, 4, 157680000));

  /* compute mcdindex measure */
  printf("mCD index: %f\n", mcdindex(&g, 4, 157680000));

  /* free memory use by the graph */
  free_graph(&g);

  return 0;
}
