# -*- encoding: utf-8 -*-
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

def extrema2attribute(n, ext):
    
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
    Bc2 = Bc.copy()
    center = np.array(Bc.shape)/2
    Bc2[tuple(center)] = 0
    off = np.transpose(Bc2.nonzero()) - center
    return np.ascontiguousarray(off, dtype = np.int32)

def SSIMIndex(X,Y, k1 = 0.01, k2 = 0.03):
    """
    Structural similarity index. Implementation based on the paper: Image Quality Assessment:
    From Error Visibility to Structural Similarity. k1 = 0.01, k2 = 0.03 are the
    default values used in the paper.
    """

    X = X.astype(float)
    Y = Y.astype(float)

    mu_x = X.mean()
    mu_y = Y.mean()

    sigma_xy = 1.0/(X.size - 1)*((X.ravel() - mu_x)*(Y.ravel() - mu_y)).sum()
    sigma_y = 1.0/(Y.size - 1)*((Y.ravel() - mu_y)** 2).sum()
    sigma_y = np.sqrt(sigma_y)
    sigma_x = 1.0/(X.size - 1)*((X.ravel() - mu_x)** 2).sum()
    sigma_x = np.sqrt(sigma_x)

    C1 = (k1*255)*(k1*255)
    C2 = (k2*255)*(k2*255)
    ssim = (2*mu_x*mu_y + C1)*(2*sigma_xy + C2)/((mu_x*mu_x + mu_y*mu_y + C1)\
           *(sigma_x*sigma_x + sigma_y*sigma_y + C2))
    return ssim
