#!/usr/local/bin/python
# -*- coding: utf-8 -*-

"""time_utilities.py: Utility functions for converting time formats."""

__author__ = "Russell J. Funk"
__copyright__ = "Copyright (C) 2017"

# built in modules
import datetime
import calendar

def timestamp_from_datetime(dt):
  """Convert a datetime to a Unix timestamp.

  Given a datetime, this function returns a Unix timestamp. Input is assumed to
  be Greenwich Mean Time (GMT) and local time zone information is ignored. The
  function is useful for converting dates to integers, which are required as
  input to some other functions in the module.

  Parameters
  ----------
  dt : datetime
    A datetime.

  Returns
  -------
  int
    A Unix timestamp.
  """
  return calendar.timegm(dt.utctimetuple())

def datetime_from_timestamp(ts):
  """Convert a Unix timestamp to a datetime.

  Given a Unix timestamp, this function returns a datetime. Input is assumed to
  be Greenwich Mean Time (GMT) and local time zone information is ignored. The
  function is useful for converting integers, which are required by some
  functions in the module, to more human readable dates.

  Parameters
  ----------
  ts : int
    A Unix timestamp.

  Returns
  -------
  datetime
    A datetime.
  """
  return datetime.datetime.utcfromtimestamp(ts)

def main():

  return None

if __name__ == "__main__":
  main()