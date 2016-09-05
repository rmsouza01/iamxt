# -*- encoding: utf-8 -*-

# BSD 2-Clause License
# Copyright (c) 2016, Roberto Souza and collaborators
# All rights reserved.

# Module draw_1D_image
import warnings
import numpy as np
try:
    import gvgen
except:
    warnings.warn("gvgen not installed, this may cause a problem in drawing functions")

try:
    import StringIO
except:
    warnings.warn("StringIO not installed, this may cause a problem in drawing functions")


def create1DImage(img):
    """
    ((1xn)-array) (int) -> (pixels_sizexn*pixels_size)-array)
    This function recieves a (1 x n) 1D image with pixels intensities between
    0 and 7 and returns a pixels image ready for display. pixels_size define
    the size of the pixels.
    """

    pixels_size = 25
    img1D = img.copy()
    img1D = np.repeat(img1D*35, pixels_size,axis = 1)
    img1D = np.repeat(img1D,pixels_size,axis = 0)

    #Drawing yellow lines to separate the pixels
    img1D = np.array([img1D,img1D,img1D])
    img1D[2,:,::pixels_size] = 0
    img1D[2,::pixels_size,:] = 0
    img1D[0,::pixels_size,:] = 255
    img1D[0,:,::pixels_size] = 255
    img1D[1,::pixels_size,:] = 255
    img1D[1,:,::pixels_size] = 255
    img1D[2,:,-1] = 0
    img1D[2,-1,:] = 0
    img1D[0,-1,:] = 255
    img1D[0,:,-1] =255
    img1D[1,-1,:] = 255
    img1D[1,:,-1] = 255
    return img1D.transpose(1,2,0) #OpenCV convention H,W,Channel

#This function implements the algorithm described in:
# R. Souza, L. Rittner, R. Machado, R. Lotufo: A comparison between extinction
# filters and attribute filters. In: International Symposium on Mathematical
# Morphology, 2015, Reykjavik.
def extrema2attribute(n, ext):
    """
    This method computes the attribute parameter to be used in the attribute
    filter that tries to preserve n extrema in the image. If it is not possible  
    to preserve n extrema, it will give preserve the closest value from n from
    below. 
    """
    
    ext = ext[ext!=0] # Non-zero extinction values
    temp = np.unique(ext) # Returns non-repeated elements sorted
    max_ext = temp[-1]
    bins = np.zeros(len(temp)+2, dtype = np.int32)
    bins[1:-1] = temp
    bins[-1] = max_ext + 1
    hist, bins = np.histogram(ext.flatten(),bins = bins)
    x = bins[:-1]
    y = (hist.sum() - np.cumsum(hist))
    index = np.where(y >= n)[0][-1]
    attr_value =  x[index]
    new_n = y[index]
    return attr_value,new_n


def generateGraph(par,img):
   """
   This method generates the graphviz code to draw the the pixel oriented
   max-tree. 
   Input: 
   - par, 1d-array, int32. Parent array.
   - img, 2d-array, uint8. Image.
   Output:
   - graphviz_tree, str. String containing the graphviz code that will be used to
     draw the pixel oriented max-tree.
   """
   f = img.ravel()
   G = gvgen.GvGen()
   G.styleAppend("levroot", "color", "red")
   G.styleAppend("levroot", "style", "filled")
   G.styleAppend("levroot", "fontcolor", "white")

   l = []
   items = {}
   for i,p in enumerate(f):
       if i != par[i]:
           l.append(i)
           items[i] = G.newItem('%d: %f' % (i,f[i]) )
           if par[i] == i or f[i] != f[par[i]]:
               G.styleApply("levroot", items[i])

   for i in l:
       p = i
       while(p != par[p]):

           if not p in items.keys():
               items[p] = G.newItem('%d:%f' % (p,f[p]))
               if par[p] == p or f[p] != f[par[p]]:
                   G.styleApply("levroot", items[p])

           if not par[p] in items.keys():
               q = par[p]
               items[q] = G.newItem('%d:%f' % (q,f[q]))
               if par[q] == q or f[q] != f[par[q]]:
                   G.styleApply("levroot", items[q])

           G.newLink(items[p], items[par[p]])
           p = par[p]

       G.newLink(items[p], items[par[p]])

   
   fd = StringIO.StringIO()
   G.dot(fd)
   dottext = fd.getvalue()
   return dottext



def se2off(Bc):
    """
    This method returns the array of offsets corresponding to the structuring
    element Bc.
    """
    Bc2 = Bc.copy()
    center = np.array(Bc.shape)/2
    Bc2[tuple(center)] = 0
    off = np.transpose(Bc2.nonzero()) - center
    return np.ascontiguousarray(off, dtype = np.int32)


