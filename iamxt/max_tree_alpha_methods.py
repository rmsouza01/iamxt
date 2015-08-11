# -*- encoding: utf-8 -*-


import numpy as np
import cv2
import os

def computeHeight(self):
    """
    This method returns the height attribute of the max-tree nodes.
    """
    parent = self.node_array[0,:]
    level  = self.node_array[2,:]
    height = np.where(self.node_array[1,:] == 0, 1, 0)
    height = height.astype(np.int32)
    delta = (level - level[parent]).astype(np.int32)
    self.compute_height_aux(parent,delta,height)
    return height



def computeVolume(self):
    """
    This method returns the volume attribute of the max-tree nodes.
    """
    parent = self.node_array[0,:]
    level  = self.node_array[2,:]
    area   = self.node_array[3,:]
    volume = area.copy()
    delta = (level - level[parent]).astype(np.int32)
    self.compute_volume_aux(parent,delta,area,volume)
    return volume

def computeExtinctionValues(self,attrib_orig, opt = "area"):
    """
    This method computes the "hegiht", "area", "volume" or "bb" extinction
    values of the Max-Tree leaves. It receives the array attrib containing
    the values to be analysed and a string with the attribute option, since
    for "height" and "volume" attributes the hidden layers of the composite
    nodes have to be considered.
    """

    attrib = attrib_orig.copy()
    parent = self.node_array[0,:]
    nchild = self.node_array[1,:]
    h = self.node_array[2,:]
    area = self.node_array[3,:]
    nlevels = h - h[parent]
    composite_nodes = nlevels > 1
    if opt == "area":
        pass
    elif opt == "height":
        attrib[composite_nodes] = attrib[composite_nodes] + \
                                  (nlevels[composite_nodes] - 1)
    elif opt == "volume":
        attrib[composite_nodes] = attrib[composite_nodes] +\
                                  (nlevels[composite_nodes] - 1)\
                                  *area[composite_nodes]
    else:
        print "Invalid Option"
        return

    ichmax = np.zeros_like(parent)
    achmax = np.zeros_like(parent)
    ext_values = np.zeros_like(parent)
    leaves = (np.nonzero(nchild == 0)[0]).astype(np.int32)
    self.compute_extinction_values_aux(parent, attrib, leaves, ichmax,\
                                       achmax, ext_values)
    return ext_values

def computeStabilityMeasure(self, delta = 5):
  """
  This method computes the MSER stability measure of the Max-Tree nodes. The parameter delta
  is the number of thresholds to be considered in the stability computation.
  """

  h = self.node_array[2,:]
  parent = self.node_array[0,:]
  area = self.node_array[3,:]
  nlevels =  (h - h[parent]).astype(np.int32)
  nlevels[0] = 1
  hmin = int(h.min())
  stability_measure = np.ones(self.node_array.shape[1], dtype = np.float)
  nodes_list = (np.nonzero( (h >= (hmin + delta)))[0]).astype(np.int32)

  self.compute_stability_measure_aux(parent, h, area, nlevels, nodes_list, stability_measure, delta, hmin)
  return stability_measure




def draw1DImageTree(self, tree = 'mt'):
  pixels_size = 25
  P = self.node_array[0,:]
  H = self.node_array[2,:]
  Ymin = self.node_array[8,:]
  Ymax = self.node_array[9,:]
  Xmin = self.node_array[5,:]
  Xmax = self.node_array[6,:]

  hmin = H.min()
  nglevels = H.max() - hmin  + 1  # Number of grey-levels in the 1D input image
  npixels = Ymax.max() + 1

  # Pre-allocating an 2d-array to store the tree image
  tree_image = np.ones((nglevels,npixels), dtype = 'uint8')*255
  # Pre-allocating an 2d-array to store the centroids of the tree nodes
  nodes_image = np.zeros((nglevels,npixels), dtype = 'uint8')
  tree_connections = []
  connection = [0,0,0,0]
  composite_nodes = []
  self.getSubBranches(0)

  for i in xrange(self._cum_sb_hist.size-1, 0,-1):
    sb =  self._sb[self._cum_sb_hist[i-1]:self._cum_sb_hist[i]]
    for j in sb:
      h = H[j]
      p = P[j]
      hp = H[p] # Grey-level of the parent

      ymin,ymax = Ymin[j],Ymax[j]
      LC = self.recConnectedComponent(j).flatten() # Recovered level component
      tree_image[(h - hmin),ymin:ymax+1]= LC[ymin:ymax+1]*h*35.0

      ys = (ymin+ymax+1)/2

      connection[0] = h - hmin
      connection[1] = ys
      nodes_image[(h-hmin),ys] = 2

      if hp != (h-1):
        composite_nodes.append([h-hmin,ys])
        for k in xrange(hp+1,h):
          tree_image[k - hmin,ymin:ymax+1] = LC[ymin:ymax+1]*k*35.0
          if tree == 'ct':
            nodes_image[k-hmin,ys] = 2
            connection[2] = hp - hmin + 1
            connection[3] = ys
            tree_connections.append(connection[:]) # [:]?
            connection[0] = hp - hmin + 1
            connection[1] = ys
      connection[2] = hp - hmin
      connection[3] =  (Ymin[p]+Ymax[p]+1)/2
      tree_connections.append(connection[:])

  tree_image = np.repeat(tree_image,pixels_size, axis = 1)
  tree_image = np.repeat(tree_image,pixels_size, axis = 0)

  new_nodes_image = np.zeros((nglevels*pixels_size,npixels*pixels_size), dtype = 'uint8')
  new_nodes_image[pixels_size/2::pixels_size,pixels_size/2::pixels_size] = nodes_image

  new_nodes_imagek = (new_nodes_image == 2).astype(np.uint8)
  disk = np.ones((5,5),np.uint8)
  new_nodes_imagek = cv2.dilate(new_nodes_imagek,disk,iterations = 1) > 0


  if tree == 'mt':
    bool_img = tree_image != 255
    bool_img2 = bool_img.copy()
    bool_img2[::2,:] = 0
    for kk in xrange(nglevels-1):
      bool_img[kk*pixels_size:(kk+1)*pixels_size,:] = bool_img[kk*pixels_size:(kk+1)*pixels_size,:] - \
                                                     bool_img2[(kk+1)*pixels_size:(kk+2)*pixels_size,:]
      tree_image[~bool_img] = 255

  tree_image = np.array([tree_image,tree_image,tree_image])
  tree_image[0][new_nodes_imagek] = 255
  tree_image[1][new_nodes_imagek] = 0
  tree_image[2][new_nodes_imagek] = 0


  if tree == 'mt':
    r = 9
    t = np.arange(0,1.01,.01)*2*np.pi
    for ii in composite_nodes[1:]:
      x,y = pixels_size/2+pixels_size*ii[1],pixels_size/2+pixels_size*ii[0]
      xc = (np.round(r*np.cos(t)) + x).astype(int)
      yc = (np.round(r*np.sin(t)) + y).astype(int)
      tree_image[0,yc,xc] = 255
      tree_image[1,yc,xc] = 0
      tree_image[2,yc,xc] = 0


  tree_image = tree_image.transpose(1,2,0) 
  tree_image[:,::pixels_size,2] = 0
  tree_image[::pixels_size,:,2] = 0
  tree_image[::pixels_size,:,0] = 255
  tree_image[:,::pixels_size,0] = 255
  tree_image[::pixels_size,:,1] = 255
  tree_image[:,::pixels_size,1] = 255
  tree_image[:,-1,2] = 0
  tree_image[-1,:,2] = 0
  tree_image[-1,:,0] = 255
  tree_image[:,-1,0] =255
  tree_image[-1,:,1] = 255
  tree_image[:,-1,1] = 255

  tree_image = np.ascontiguousarray(tree_image)
  for jj in tree_connections:
    pt1 = (pixels_size/2+pixels_size*jj[1],pixels_size/2+pixels_size*jj[0])
    pt2 = (pixels_size/2+pixels_size*jj[3],pixels_size/2+pixels_size*jj[2])
    cv2.line(tree_image,pt1,pt2, color = [255,0,0])
  tree_image = tree_image[::-1,:,:]  

  
  _ROOT = os.path.abspath(os.path.dirname(__file__))
  text = cv2.imread(os.path.join(_ROOT,'things','grey_levels.png'))
  return np.concatenate((text[pixels_size*(10-nglevels):,:,:],tree_image), axis = 1)


def extinctionFilter(self,ext,n):
    """
    This method implements the Extinction Filter. It keeps the n most
    relevant extrema according to the extinction values ext.
    """

    ii = (np.argsort(ext)[::-1][:n]).astype(np.int32)
    to_remove = np.ones((self.node_array.shape[1],),dtype = np.int32)
    self.extinction_filter_aux(to_remove,ii,self.node_array[0,:])
    self.prune(to_remove.astype(bool))
    return self


def getSignature(self,attrib, start, end = 0, cte = 0):
    """
    This method returns a tuple containing the gray-levels and the
    attribute signature. Composite nodes are considered.
    cte:
    0 -> shape signature
    1 -> volume signature
    """
    par = self.node_array[0,:]
    h = self.node_array[2,:]
    area = self.node_array[3,:]
    attrib = attrib.astype(np.int32)
    signature = self.get_signature_aux(par, h, area, attrib, int(start), int(end), int(cte))
    levels = np.arange(h[end], h[start] + 1, dtype = np.int32)
    return levels, signature

def hmax(self, h, Height = None):
    """
    This method implemnets the hmax filter
    """
    h = h + 1
    if Height == None:
        child_height = self.computeHeight() - 1
    else:
        child_height = Height -1

    level = self.node_array[2,:]
    parent = self.node_array[0,:]
    total_height = level - level[parent] + child_height
    self.prune(total_height < h)
    child_height = child_height[total_height > h]
    self.node_array[2,child_height < h] -= (h - child_height[child_height < h])
    return self

def vmax(self, vol,V = None):
    """
    This method implemnets the vmax filter
    """
    vol = vol + 1
    area = self.node_array[3,:]
    level = self.node_array[2,:]
    parent = self.node_array[0,:]
    if V == None:
        child_volume = self.computeVolume() - area
    else:
        child_volume = V - area

    total_volume = child_volume + (level - level[parent])*area
    self.prune(total_volume < vol)
    area = area[total_volume > vol]
    child_volume = child_volume[total_volume > vol]
    self.node_array[2,child_volume < vol] -= (vol - child_volume[child_volume < vol]) / area[child_volume < vol]
    return self

def mmsMSER(self,stability_measure):
    if not self._sb_updated:
        self.getSubBranches()  # List of sub-branches

    to_keep = np.zeros(self._cum_sb_hist.size, dtype = np.int32)
    self.mms_mser_aux(stability_measure, to_keep, self._sb,self._cum_sb_hist)
    bool_tokeep = np.zeros(self.node_array.shape[1],dtype = bool)
    bool_tokeep[to_keep] = True
    self.contractDR(bool_tokeep)
    return

def mmsT(self, t = 0.5):
    if not self._sb_updated:
        self.getSubBranches()  # List of sub-branches
    to_keep = np.zeros(self._cum_sb_hist.size, dtype = np.int32)

    new_h = np.zeros(self._cum_sb_hist.size, dtype = np.int32)
    h = self.node_array[2,:]
    parent = self.node_array[0,:]
    nlevels =  (h - h[parent]).astype(np.int32)
    nlevels[0] = 1

    self.mms_t_aux(t, nlevels, h, to_keep, new_h,self._sb,self._cum_sb_hist )
    h[to_keep] = new_h
    bool_tokeep = np.zeros(self.node_array.shape[1],dtype = bool)
    bool_tokeep[to_keep] = True
    self.contractDR(bool_tokeep)
    return

