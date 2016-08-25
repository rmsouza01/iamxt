#! /usr/bin/env python

# System imports
from distutils.core import *
from distutils      import sysconfig

import os

(opt,) = sysconfig.get_config_vars('OPT')
os.environ['OPT'] = " ".join(flag for flag in opt.split() if flag != '-Wstrict-prototypes')




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
            define_macros=[('NDEBUG', None),('NPY_NO_DEPRECATED_API', 'NPY_1_7_API_VERSION')],
            include_dirs=['/usr/local/include/opencv',numpy_include],
            extra_compile_args=["-fopenmp"],
            extra_link_args=["-fopenmp"],
            library_dirs=[],
            libraries=[],
            swig_opts=['-c++', '-includeall', '-threads', '-keyword'],
        )

_morph_tree_alpha_aux_c = Extension(
            '_morph_tree_alpha_aux',
            ['iamxt/morph_tree_alpha_aux.cpp', 'iamxt/morph_tree_alpha_aux.i'], 
            define_macros=[('NDEBUG', None),('NPY_NO_DEPRECATED_API', 'NPY_1_7_API_VERSION')],
            include_dirs=['/usr/local/include/opencv',numpy_include],
            extra_compile_args=["-fopenmp"],
            extra_link_args=["-fopenmp"],
            library_dirs=[],
            libraries=[],
            swig_opts=['-c++', '-includeall', '-threads', '-keyword'],
        )


_max_tree_alpha_aux_c = Extension(
            '_max_tree_alpha_aux',
            ['iamxt/max_tree_alpha_aux.cpp', 'iamxt/max_tree_alpha_aux.i'], 
            define_macros=[('NDEBUG', None),('NPY_NO_DEPRECATED_API', 'NPY_1_7_API_VERSION')],
            include_dirs=['/usr/local/include/opencv',numpy_include],
            extra_compile_args=["-fopenmp"],
            extra_link_args=["-fopenmp"],
            library_dirs=[],
            libraries=[],
            swig_opts=['-c++', '-includeall', '-threads', '-keyword'],
        )

setup(  name        = "iamxt",
        version     = "0.1",
	ext_modules = [_max_tree_c_01,_morph_tree_alpha_aux_c,_max_tree_alpha_aux_c],
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


