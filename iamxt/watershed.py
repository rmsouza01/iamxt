import numpy as np
from aux import se2off
import watershed_c
import warnings

def ws_markers(img,markers,Bc):
	"""
	IFT watershed from markers.
	Input:
	img -> 2d or 3d, uint8 or uint16 image
	markers ->  uint16 image, same number of dimensions of img
	Bc -> Boolean array defining connectivity, same number of dimensions as img.
	Output:
	seg -> uint16 image same number of dimensions as img
	"""

	D2 = False
        if img.ndim == 2:
            temp_H,temp_W = img.shape
            img = img.reshape(1,temp_H,temp_W)
            markers = markers.reshape(1,temp_H,temp_W)
            D2 = True
        off = se2off(Bc) #offsets
        
        if off.shape[1] == 2:
            off = np.concatenate((np.zeros((off.shape[0],1), dtype = np.int32),off), axis = 1)
        
	ftype = img.dtype
	mtype = markers.dtype
	if (mtype != np.int32):
		warnings.warn("markers should be int32, forcing conversion")
		markers = markers.astype(np.int32)
	if (ftype == np.uint8):
		img = img.astype(np.uint16)	

        seg = markers.copy()
        ii32 = np.iinfo(np.int32).max # infinity
        P = np.empty(img.shape, dtype = np.int32)
        P[:] = -1
        C1 = np.empty(img.shape, dtype = np.int32)
        C1[:] = ii32
        C2 = np.zeros(img.shape, dtype = np.int32)
        done =np.zeros(img.shape, dtype = np.int32)
        watershed_c.ws_markers_c(ii32,off,img,seg,C1,C2,done,P)

        if D2:
            L,M,N = seg.shape
            seg = seg.reshape(M,N)
        return seg 

def tz_ws(img,markers,Bc):
	"""
	Tie-zone watershed from markers.
	Input:
	img -> 2d or 3d, uint8 or uint16 image
	markers ->  uint16 image, same number of dimensions of img
	Bc -> Boolean array defining connectivity, same number of dimensions as img.
	Output:
	seg -> uint16 image same number of dimensions as img. The tie-zones receive the label 0.
	"""
        
        D2 = False
        if img.ndim == 2:
            temp_H,temp_W = img.shape
            img = img.reshape(1,temp_H,temp_W)
            markers = markers.reshape(1,temp_H,temp_W)
            D2 = True
        off = se2off(Bc) #offsets
        
        if off.shape[1] == 2:
            off = np.concatenate((np.zeros((off.shape[0],1), dtype = np.int32),off), axis = 1)

       	ftype = img.dtype
	mtype = markers.dtype
	if (mtype != np.int32):
		warnings.warn("markers should be int32, forcing conversion")
		markers = markers.astype(np.int32)
	if (ftype == np.uint8):
		img = img.astype(np.uint16)	

        seg = markers.copy()
        ii32 = np.iinfo(np.int32).max # infinity
        P = np.empty(img.shape, dtype = np.int32)
        P[:] = -1
        C1 = np.empty(img.shape, dtype = np.int32)
        C1[:] = ii32
        C2 = np.zeros(img.shape, dtype = np.int32)
        done =np.zeros(img.shape, dtype = np.int32)
        watershed_c.tz_ws_c(ii32,off,img,seg,C1,C2,done,P)

        if D2:
            L,M,N = seg.shape
            seg = seg.reshape(M,N)
        return seg 




