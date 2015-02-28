#!/usr/bin/env python
"""
setup.py file for rfidtag example
"""

from distutils.core import setup, Extension


rfidtag_module = Extension('_rfidtag',
                           sources=['src/rfidtag.c','src/rfidtag.i'],
                           libraries=["usb"],
                           )

# This code was originally by Younghun Kim, Thomas Schmid, Aaron Dunn.
# I've since packaged it up and extended it to support touchatags - hence
# the rename.
#
# Michael Sparks

setup (name = 'rfidtag',
       version = '0.2.1',
       author      = "Younghun Kim, Thomas Schmid, Aaron Dunn",
       author_email = "thomas.schmid@gmail.com",
       maintainer = "Michael Sparks",
       maintainer_email = "sparks.m@gmail.com",
       license = "GPL V2",
       description = """rfidtag - access to tikitag and touchatag tags from python""",
       ext_modules = [rfidtag_module],
       py_modules = ["rfidtag"],
       package_dir = {'': 'src'},
       url = "http://groups.google.com/group/tikitag/browse_thread/thread/c3aa678690800240"
       )
