# -*- encoding: utf-8 -*-



from aux import se2off
import numpy as np
import max_tree_c_01

def build_max_tree(f, Bc, option = 0):

    ndim = f.ndim
    off = se2off(Bc) # Array of offsets

    parent = np.empty(f.size, dtype = np.int32)
    parent[:] = -1

    zpar = np.empty(f.size, dtype = np.int32)

    flat_img = f.ravel()
    S_rev = max_tree_c_01.counting_sort_c(flat_img)
    if ndim == 2:
        H,W = f.shape
        max_tree_c_01.union_find2d_c(H,W,off,parent,zpar,S_rev,flat_img)
    elif ndim == 3:
        L,M,N = f.shape
        max_tree_c_01.union_find3d_c(L,M,N,off,parent,zpar,S_rev,flat_img)
    else:
        print "Invalid option"
        return

    max_tree_c_01.canonicalize_c(flat_img,parent,S_rev)

    if option == 0:
        return parent,S_rev
    else:
        node_index = np.empty(f.shape, dtype = np.int32)
        node_index[:] = -1
        if ndim == 2:
            node_array = max_tree_c_01.computeNodeArray2d_c(parent,flat_img,S_rev,node_index)
        else:
            node_array = max_tree_c_01.computeNodeArray3d_c(parent,flat_img,S_rev,node_index)

        node_array = node_array.T.copy()
        return parent,S_rev,node_array,node_index

def compute_area(S,parent):
    area = np.ones(S.size, dtype = np.int32)
    max_tree_c_01.compute_area_c(S,parent,area)
    return area

def direct_filter(lambda_,S,parent,img,attr):
    flat_img = np.ascontiguousarray(img.ravel())
    out = np.empty(S.size, dtype = np.uint8)
    max_tree_c_01.direct_filter_c(lambda_,S,parent,flat_img,out,attr.astype(float))
    return out.reshape(img.shape)
