#!/usr/local/bin/python
# -*- coding: utf-8 -*-

"""setup.py: This script installs the cdindex python module."""

__author__ = "Russell J. Funk"
__copyright__ = "Copyright (C) 2017"

# built in modules
from setuptools import setup, Extension, find_packages

setup(name="cdindex",
    version="1.0.7",
    description="Package for computing the cdindex.",
    author="Russell J. Funk",
    author_email="russellfunk@gmail.com",
    url="http://www.cdindex.info",
    license="GNU General Public License (GPL)",
    ext_modules=[
                  Extension("cdindex._cdindex",
                            ["src/cdindex.c", 
                             "src/graph.c", 
                             "src/utility.c", 
                             "cdindex/pycdindex.c"],
                             include_dirs = ["src"],
                           )
                ],
    packages=find_packages()
)

# python setup.py build_ext --inplace
# python setup.py sdist