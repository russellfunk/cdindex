cdindex
=======

.. image:: https://readthedocs.org/projects/cdindex/badge/?version=latest
   :target: https://readthedocs.org/projects/cdindex/?badge=latest
   :alt: Documentation Status

cdindex is a Python package, written in C, for computing the CD index and other dynamic 
measures on evolving directed graphs.

- **Website:** http://www.cdindex.info
- **Source:** https://github.com/russellfunk/cdindex
- **Bug reports:** <russellfunk@gmail.com>

Install
-------

Install the latest version of cdindex::

    $ pip install cdindex

Simple example
--------------

Create a graph with some dummy data and compute the CD index::

    >>> import cdindex
    >>> import datetime
    >>> from dateutil.relativedelta import relativedelta

    >>> def get_t_delta(vertex, years):
          vertex_datetime = cdindex.datetime_from_timestamp(graph.timestamp(vertex))
          future_datetime = vertex_datetime + relativedelta(years=years)
          difference = future_datetime - vertex_datetime
          return int(difference.total_seconds())

    >>> # dummy vertices for python module tests
    >>> pyvertices= [{"name": "0Z", "time": datetime.datetime(1992, 1, 1)},
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

    >>> # dummy edges for python module tests
    >>> pyedges = [{"source": "4Z", "target": "2Z"},
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
 

    >>> # create graph
    >>> graph = cdindex.Graph()

    >>> # add vertices
    >>> for vertex in pyvertices:
          graph.add_vertex(vertex["name"], cdindex.timestamp_from_datetime(vertex["time"]))

    >>> # add edges
    >>> for edge in pyedges:
          graph.add_edge(edge["source"], edge["target"])

    >>> graph.cdindex("4Z", get_t_delta("4Z", years=5))

    >>> graph.mcdindex("4Z", get_t_delta("4Z", years=5))

Bugs
----

Please report any bugs to <russellfunk@gmail.com>.

License
-------

Released under the GNU General Public License (GPL) (see `LICENSE`)::

   Copyright (C) 2024 Russell J. Funk <russellfunk@gmail.com>
   
Other implementations
---------------------

Below is a list of other (known) implementations of the cdindex. Please contact us if you know of others.

* novelpy (https://novelpy.readthedocs.io/en/latest/index.html)
* fast-cdindex (https://github.com/dspinellis/fast-cdindex)
