#!/usr/local/bin/python
# -*- coding: utf-8 -*-

"""cdindex.py: This script is an interface for the c extension _cdindex."""

__author__ = "Russell J. Funk"
__copyright__ = "Copyright (C) 2017"

# built in modules
import math
import random
import itertools
import os

# custom modules
if "READTHEDOCS" not in os.environ:
  import _cdindex
import time_utilities

class Graph:
  """Create a graph.

  This class defines a graph data structure on which to compute the cdindex and other
  functions that are made available through the module.
  """  
  
  def __init__(self, vertices=[], edges=[]):
    """Initialize a new graph.

    Create a new graph for analysis. Vertices and edges can be added either when the graph
    is initialized, or later. 
    
    Example
    -------
    vs = [{"name": "0Z", "time": datetime.datetime(1992, 1, 1)},
          {"name": "1Z", "time": datetime.datetime(1992, 1, 1)},
          {"name": "2Z", "time": datetime.datetime(1993, 1, 1)},
          {"name": "3Z", "time": datetime.datetime(1993, 1, 1)},
          {"name": "4Z", "time": datetime.datetime(1995, 1, 1)},
          {"name": "5Z", "time": datetime.datetime(1997, 1, 1)},
          {"name": "6Z", "time": datetime.datetime(1998, 1, 1)},
          {"name": "7Z", "time": datetime.datetime(1999, 1, 1)},
          {"name": "8Z", "time": datetime.datetime(1999, 1, 1)},
          {"name": "9Z", "time": datetime.datetime(1998, 1, 1)},
          {"name": "10Z", "time": datetime.datetime(1997, 1, 1)}]

    es = [{"source": "4Z", "target": "2Z"},
          {"source": "4Z", "target": "0Z"},
          {"source": "4Z", "target": "1Z"},
          {"source": "4Z", "target": "3Z"},
          {"source": "5Z", "target": "2Z"},
          {"source": "6Z", "target": "2Z"},
          {"source": "6Z", "target": "4Z"},
          {"source": "7Z", "target": "4Z"},
          {"source": "8Z", "target": "4Z"},
          {"source": "9Z", "target": "4Z"},
          {"source": "9Z", "target": "1Z"},
          {"source": "9Z", "target": "3Z"},
          {"source": "10Z", "target": "4Z"}]

    graph = cdindex.Graph(vertices=vs, edges=es)
    

    Parameters
    ----------
    vertices :
      List of vertices with names and timestamps, e.g., 
      [{"name": "0Z", "time": datetime.datetime(1992, 1, 1)}].
    edges : 
      List of edges with sources and targets, e.g., [{"source": "4Z", "target": "2Z"}].
    """

    self._graph = _cdindex.Graph()
    self._vertex_name_crosswalk = {}
    self._vertex_id_crosswalk = {}

    # add vertices
    for vertex in vertices:
      self.add_vertex(vertex["name"], vertex["time"])

    # add edges
    for edge in edges:
      self.add_edge(edge["source"], edge["target"])
    
  def add_vertex(self, name, t):
    """Add a new vertex to the graph.

    This function adds a new vertex to the graph, given a name and timestamp.

    Parameters
    ----------
    name :
      The vertex name.
    t : int
      The vertex timestamp.
    """
    vertex_id = self.vcount()
    if name in self._vertex_name_crosswalk:
     raise ValueError("Vertex already added to graph")
    else:
      self._vertex_name_crosswalk[name] = vertex_id
      self._vertex_id_crosswalk[vertex_id] = name
    if isinstance(t, (long, int)) is False:
      raise ValueError("Time (t) of vertex must be an integer or long")

    # add the vertex
    _cdindex.add_vertex(self._graph, vertex_id, t)

  def add_edge(self, source_name, target_name):
    """Add a new edge to the graph.

    This function adds a new edge to the graph, given a source name and target
    name.

    Parameters
    ----------
    source_name :
      The source vertex name.
    target_name :
      The target vertex timestamp.
    """
    if (source_name not in self._vertex_name_crosswalk
        or target_name not in self._vertex_name_crosswalk):
      raise ValueError("One or more vertices are not in the graph")
    elif (target_name in self.out_edges(source_name)):
      raise ValueError("The edge being added is already in the graph")
    else:
      _cdindex.add_edge(self._graph,
                        self._vertex_name_crosswalk[source_name],
                        self._vertex_name_crosswalk[target_name])

  def vcount(self):
    """Return the number of vertices in the graph.

    This function returns the number of vertices in the graph. Note that time is
    not taken into account. The total count of vertices are returned regardless
    of when they appear in the graph.

    Returns
    -------
    int
      The number of vertices.
    """
    return _cdindex.get_vcount(self._graph)

  def _c_vertices(self):
    """Return the ids of the vertices in the graph from the C extension.

    This function returns a list of vertices in the graph. Rather than returning
    names, the id numbers from the C extension are returned. This function is
    not useful other than for internal purpuses. Note that time is not taken
    into account. Vertices are returned regardless of when they appear in the
    graph.

    Returns
    -------
    list
      The vertices.
    """
    return _cdindex.get_vertices(self._graph)

  def vertices(self):
    """Return the vertices in the graph.

    This function returns a list of vertices in the graph. Note that time is not
    taken into account. Vertices are returned regardless of when they appear in
    the graph.

    Returns
    -------
    list
      The vertices.
    """
    return self._vertex_name_crosswalk.keys()

  def ecount(self):
    """Return the number of edges in the graph.

    This function returns the number of edges in the graph. Note that time is
    not taken into account. The total count of edges are returned regardless of
    when they appear in the graph.

    Returns
    -------
    int
      The number of edges.
    """
    return _cdindex.get_ecount(self._graph)

  def in_degree(self, name):
    """Return the in degree of the focal vertex.

    Given a vertex, this function returns its in degree centrality, i.e., the
    number of other vertices in the graph that cite the focal vertex. Note that
    time is not taken into account. The function counts edges regardless of when
    they appear in the graph.

    Parameters
    ----------
    name :
      The vertex name.

    Returns
    -------
    int
      The in degree centrality.
    """
    return _cdindex.get_vertex_in_degree(self._graph,
                                         self._vertex_name_crosswalk[name])

  def in_edges(self, name):
    """Return the in edges of the focal vertex.

    Given a vertex, this function returns its in edges, i.e., the other
    vertices in the graph cited by the focal vertex. Note that time is not taken
    into account. The function returns edges regardless of when they appear in
    the graph.

    Parameters
    ----------
    name :
      The vertex name.

    Returns
    -------
    list
      The in edges.
    """
    in_edges_ids = _cdindex.get_vertex_in_edges(self._graph,
                                                self._vertex_name_crosswalk[name])
    return [self._vertex_id_crosswalk[vertex_id] for vertex_id in in_edges_ids]

  def out_degree(self, name):
    """Return the out degree of the focal vertex.

    Given a vertex, this function returns its out degree centrality, i.e., the
    number of other vertices in the graph cited by the focal vertex. Note that
    time is not taken into account. The function counts edges regardless of when
    they appear in the graph.

    Parameters
    ----------
    name :
      The vertex name.

    Returns
    -------
    int
      The out degree centrality.
    """
    return _cdindex.get_vertex_out_degree(self._graph,
                                          self._vertex_name_crosswalk[name])

  def out_edges(self, name):
    """Return the out edges of the focal vertex.

    Given a vertex, this function returns its out edges, i.e., the other
    vertices in the graph cited by the focal vertex. Note that time is not taken
    into account. The function returns edges regardless of when they appear in
    the graph.

    Parameters
    ----------
    name :
      The vertex name.

    Returns
    -------
    list
      The out edges.
    """
    out_edges_ids = _cdindex.get_vertex_out_edges(self._graph,
                                                  self._vertex_name_crosswalk[name])
    return [self._vertex_id_crosswalk[vertex_id] for vertex_id in out_edges_ids]

  def timestamp(self, name):
    """Return the timestamp of the focal vertex.

    Given a vertex, this function returns its timestamp.

    Parameters
    ----------
    name :
      The vertex name.

    Returns
    -------
    int
      The timestamp.
    """
    return _cdindex.get_vertex_timestamp(self._graph,
                                         self._vertex_name_crosswalk[name])

  def cdindex(self, name, t_delta):
    """Compute the CD index.

    This function computes the CD index for a specified vertex at a given
    t_delta, where t_delta is an integer that gives the positive distance in
    time from the timestamp of the focal node when the index should be
    calculated.

    Parameters
    ----------
    t_delta : int
      A time delta.

    Returns
    -------
    double
      The CD index.
    """
    if isinstance(t_delta, (long, int)) is False:
      raise ValueError("Time delta (t_delta) must be an integer or long")
    return _cdindex.cdindex(self._graph,
                            self._vertex_name_crosswalk[name],
                            t_delta)

  def mcdindex(self, name, t_delta):
    """Compute the mCD index.

    This function computes the mCD index for a specified vertex at a given
    t_delta, where t_delta is an integer that gives the positive distance in
    time from the timestamp of the focal node when the index should be
    calculated.

    Parameters
    ----------
    t_delta : int
      A time delta.

    Returns
    -------
    double
      The mCD index.
    """
    if isinstance(t_delta, (long, int)) is False:
      raise ValueError("Time delta (t_delta) must be an integer or long")
    return _cdindex.mcdindex(self._graph,
                             self._vertex_name_crosswalk[name],
                             t_delta)

  def _is_graph_sane(self):
    """Test graph sanity.

    Run tests to verify the internal graph storage structure. Not really a
    useful function other than for checking possible issues with the module.

    Returns
    -------
    bool
      True if successful, False otherwise.
    """
    return _cdindex._is_graph_sane(self._graph)

class RandomGraph(Graph):
  """Create a random graph.

  This class generates a random graph that can be used for testing the cdindex and other
  functions that are made available through the module.
  """  

  def __init__(self, generations=[], edge_fraction=[]):
    """Initialize a new random graph.

    Create a new random graph for analysis.
    
    Example
    -------
    graph = cdindex.RandomGraph(generations=(2,3,4,5,6,7,7,9), edge_fraction=1)
    
    Parameters
    ----------
    generations :
      List of discrete "generations", i.e., timestamps for each vertex.
    edge_fraction : 
      The fraction of edges that should be present in the graph.
    """

    # build vertex list
    vertices = []
    for generation_id, generation_size in enumerate(generations):
      new_vertices = range(generation_size)
      vertices.extend([{"name": "v" + str(nv) + "g" + str(generation_id), 
                        "time": generation_id} for nv in new_vertices])

    # build edge list
    edges = []
    for possible_edges in itertools.product(vertices, vertices):
      if possible_edges[0]["time"] > possible_edges[1]["time"]:
        edges.append({"source": possible_edges[0]["name"], 
                      "target": possible_edges[1]["name"]})

    # pull fraction of edges to keep
    random.shuffle(edges)
    edges = edges[0:int(math.ceil(edge_fraction * len(edges)))]

    # initialize graph
    Graph.__init__(self, vertices, edges)

def main():

  return None

if __name__ == '__main__':
  main()
