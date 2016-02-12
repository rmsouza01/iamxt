#! /usr/bin/env python

# System imports
from distutils.core import *
from distutils      import sysconfig

# Third-party modules - we depend on numpy for everything
import numpy

# Obtain the numpy include directory.  This logic works across numpy versions.
try:
    numpy_include = numpy.get_include()
except AttributeError:
    numpy_include = numpy.get_numpy_include() # get_numpy_include()

def read(fname):
    return open(os.path.join(os.path.dirname(__file__), fname)).read()

# extension modules
_max_tree_c_01 = Extension(
            '_max_tree_c_01',
            ['iamxt/max_tree_c_01.cpp', 'iamxt/max_tree_c_01.i'], 
            include_dirs=['/usr/local/include/opencv',numpy_include],
            define_macros=[('NDEBUG', None)],
            library_dirs=[],
            libraries=[],
            swig_opts=['-c++', '-includeall', '-threads', '-keyword'],
        )

_watershed_c =  Extension(
            '_watershed_c',
            ['iamxt/watershed_c.cpp', 'iamxt/watershed_c.i'], 
            include_dirs=[numpy_include], 
            define_macros=[('NDEBUG', None)],    
            library_dirs=[],
            libraries=[],
            swig_opts=['-c++', '-includeall', '-threads', '-keyword'],
        )


_morph_tree_alpha_aux_c = Extension(
            '_morph_tree_alpha_aux',
            ['iamxt/morph_tree_alpha_aux.cpp', 'iamxt/morph_tree_alpha_aux.i'], 
            include_dirs=['/usr/local/include/opencv',numpy_include],
            define_macros=[('NDEBUG', None)],
            library_dirs=[],
            libraries=[],
            swig_opts=['-c++', '-includeall', '-threads', '-keyword'],
        )


_max_tree_alpha_aux_c = Extension(
            '_max_tree_alpha_aux',
            ['iamxt/max_tree_alpha_aux.cpp', 'iamxt/max_tree_alpha_aux.i'], 
            include_dirs=['/usr/local/include/opencv',numpy_include],
            define_macros=[('NDEBUG', None)],
            library_dirs=[],
            libraries=[],
            swig_opts=['-c++', '-includeall', '-threads', '-keyword'],
        )

setup(  name        = "iamxt",
        version     = "0.1",
	ext_modules = [_max_tree_c_01,_morph_tree_alpha_aux_c,_max_tree_alpha_aux_c,_watershed_c],
	packages=['iamxt'],
	package_data={'iamxt': ['things/grey_levels.png']},
        #data_files=[('iamxt', ['images/cameraman.png', 'images/mri.jpg','images/lena.png','images/lp_image.png','images/itajaiacu.png'])], 
 	author="Roberto M Souza and collaborators",
        author_email="roberto.medeiros.souza@gmail.com",
        description="Max-tree Toolbox for Teaching Image Processing",
        license="BSD 2-clause License",
        keywords=["image processing", "mathematical morphology","max-tree"],
        url="https://github.com/rmsouza01/iamxt/tree/16bit/iamxt",
        long_description=read('README.txt'),
        classifiers=[
        "Development Status :: 1 - Alpha",
        "Intended Audience :: Science/Research",
        "Programming Language :: Python and C++ glued with SWIG",
        "Topic :: Scientific/Engineering :: Mathematical Morphology",
        "License :: BSD 2-clause",
        ],
        )


