==================================
iamxt: Max-tree Toolbox 
==================================

Description
-----------

This is an alpha version of the Max-tree toolbox. The toolbox was implemented in Python
and the critical functions were implemented in C++ with the parallel loops optimized using
OpenMP and wrapped in Python using SWIG. We have a different max-tree structure that is more
suitable for array processing, and allows a fast development of new methods with a reasonable
processing time. Our main goal in providing this toolbox is to spread the max-tree data structure
further among the scientific community, investigate it further, and develop new tools that may
be applied to solve real signal, image processing, and pattern recognition problems. This toolbox
works for both 2D and 3D images of type uint8 and uint16.

Authors
---------

Roberto Souza - roberto.medeiros.souza@gmail.com
Letícia Rittner - lrittner@gmail.com
Rubens Machado - rubens.campos.machado@gmail.com 
Roberto Lotufo - robertoalotufo@gmail.com 

Requirements
------------

This toolbox requires OpenMP, SWIG, OpenCV, NumPy, gvgen, and dot for rendering graphs. In fact, you only
need gvgen and dot if you intend to draw graphs.



Documentation
-------------

The documentation of this toolbox is available at:
http://adessowiki.fee.unicamp.br/adesso/wiki/iamxt/view/

Install
-------
Download the zip file, unzip it, go to the toolbox folder and type on terminal:
sudo python setup.py install


Observations
---------------

This is the first time that I try to wrap some code for distribution, therefore you'll proabably need 
to make a few modifications  on the drawing functions to have it working on your machine, specially if
you are not using Linux (ubuntu). If you have experience in making distributions and you have tips on how
to make them more portable, I'll gladly listen to them!


Contact
---------

If you have any doubts, questions or suggestions to improve this toolbox, please contact me at:
roberto.medeiros.souza@gmail.com
