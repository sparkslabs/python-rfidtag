#!/usr/bin/env python
"""
setup.py file for pytiki example
"""

from distutils.core import setup, Extension


pytiki_module = Extension('_pytiki',
                           sources=['src/tikitags.c','src/tikitags.i'],
                           libraries=["usb"],
                           )

setup (name = 'pytiki',
       version = '0.1',
       author      = "Younghun Kim, Thomas Schmid, Aaron Dunn",  # I didn't write this they did
       author_email = "thomas.schmid@gmail.com",
       maintainer = "Michael Sparks", # But I've produced this packaging so for queries regarding that contact me
       maintainer_email = "sparks.m@gmail.com",
       license = "GPL V2",
       description = """pytiki - access to tikitag tags from python""",
       ext_modules = [pytiki_module],
       py_modules = ["pytiki"],
       package_dir = {'': 'src'},
       url = "http://groups.google.com/group/tikitag/browse_thread/thread/c3aa678690800240"
       )
