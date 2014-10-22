# -*- encoding: utf-8 -*-

import numpy as np

def compute_height_aux_py(parent,delta,height):
    """
    This method returns the height attribute of the Max-Tree nodes.
    """
    N = parent.size
    for i in np.arange(N)[:0:-1]:
        height[parent[i]] = np.maximum(height[parent[i]], height[i]+delta[i])
    return

def compute_volume_aux_py(parent,delta,area,volume):
    """
    This method returns the volume attribute of the Max-Tree nodes.
    """
    N = parent.size
    for i in np.arange(N)[:0:-1]:          # scanning from leaves to root exclusive
        volume[parent[i]] += volume[i] + (delta[i] - 1)*area[i]  # this is tricky

    return volume	

def compute_extinction_values_aux_py(parent, attrib, leaves, ichmax, \
                                     achmax, ext_values):
    """
    This method computes the "hegiht", "area", "volume" or "bb" extinction
    values of the Max-Tree leaves. It receives the array attrib containing
    the values to be analysed and a string with the attribute option, since
    for "height" and "volume" attributes the hidden layers of the composite
    nodes have to be considered.
    """

    N = parent.size

    for node in np.arange(1,N)[::-1]:
        pai = parent[node]
        if attrib[node] > achmax[pai]:
            achmax[pai] = attrib[node]
            ichmax[pai] = node

    for nd in leaves:
        node = nd
        while parent[node] != node:    # while not root
            vv  = attrib[node]         # my total volume/height/area
            big = ichmax[parent[node]] # my parent biggest child
            if node != big:            # it's not me, stop the scan
                ext_values[nd] = vv
                break
            node = parent[node]        # it's me, keep on walking
        if parent[node] == node:
            ext_values[nd] = achmax[node]
    return

def compute_stability_measure_aux_py(parent, h, area, nlevels, nodes_list, stability_measure, delta, hmin):
    """
    This method computes the MSER stability measure of the Max-Tree nodes. The parameter delta
    is the number of thresholds to be considered in the stability computation.
    """

    for i in nodes_list:
        if nlevels[i] > delta:
            ancestor_index = i
        else:
            ancestor_index = parent[i]
            while True:
                if (h[i] - h[ancestor_index]+ nlevels[ancestor_index]) >delta:
                    break
                else:
                    ancestor_index = parent[ancestor_index]
        att0 = area[ancestor_index]
        att1 = area[i]
        stability_measure[i] = 1.0*(att0 - att1)/att0
    return stability_measure


def extinction_filter_aux_py(to_remove,ii,par):
    for node in ii:
        par_node = par[node]
        while(to_remove[node]!= 0):
            to_remove[node] = 0
            node = par_node
            par_node = par[node]
    return

def get_signature_aux_py(par, h, area, attrib, start, end, cte):
    """
    This method returns a tuple containing the gray-levels and
    the attribute signature. Composite nodes are considered.
    cte:
    0 -> shape signature
    1 -> volume signature
    """
    signature = np.zeros(h[start] - h[end] + 1, dtype = np.int32)
    counter = signature.size - 1
    node = start

    if (cte == 0):
        while (node!= end):
            nlevels = h[node] - h[par[node]]
            for i in xrange(nlevels):
                signature[counter] = attrib[node]
                counter-=1
            node = par[node]
        signature[0] = attrib[end]
    elif (cte == 1):
        while (node != end):
            nlevels = h[node] - h[par[node]];
            for i in xrange(nlevels):
                signature[counter] = attrib[node] + i*area[node];
                counter-=1
            node = par[node]
        signature[0] = attrib[end]
    else:
        print "Invalid option"
        return [],[]
    return  signature

def mms_mser_aux_py(stability_measure, to_keep, sb,cum_sb_hist):
    nsb = cum_sb_hist.size -1
    j = 0
    for i in xrange(nsb-1):
        sb_temp = sb[cum_sb_hist[i]:cum_sb_hist[i+1]]
        index = np.argmin(stability_measure[sb_temp])
        to_keep[j] = sb_temp[index]
        j+= 1
    return

def mms_t_aux_py(t, nlevels, h, to_keep, new_h,sb_,cum_sb_hist ):

    nsb = cum_sb_hist.size -1
    j = 0

    for i in xrange(nsb-1):
        sb = sb_[cum_sb_hist[i]:cum_sb_hist[i+1]]
        nlevels_sb = nlevels[sb]
        h_sb = h[sb]
        ngreylevels = nlevels_sb.sum() - 1
        start_node = sb[-1]
        h_new = h[start_node] - nlevels[start_node] + np.rint(1 + t*ngreylevels)
        new_h[j] = h_new
        bool_mask =  np.logical_and((h_sb - nlevels_sb) < h_new , (h_new<= h_sb))
        to_keep[j] = sb[bool_mask][0]
        j+= 1
    h[to_keep] = new_h
    return
