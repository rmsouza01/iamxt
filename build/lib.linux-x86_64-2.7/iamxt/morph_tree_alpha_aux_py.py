# -*- encoding: utf-8 -*-

import numpy as np

def contract_dr_2d_aux_py(children_list,cum_children_hist, to_keep,parent,nchild,xmin,\
                          xmax,ymin,ymax,node_index):

  AMTup =[]
  AMTup.append(children_list[0:cum_children_hist[0]].tolist())
  for i in xrange(1,parent.size):
    temp = children_list[cum_children_hist[i-1]:cum_children_hist[i]].tolist()
    AMTup.append(temp)


  to_remove = np.logical_not(to_keep)
  to_remove[0] = False

  for i in np.nonzero(to_remove)[0][::-1]:    # scanning from leaves to root
    pn = parent[i]                          # the node about to be removed has this parent
    if (nchild[i] == 0):                    # it is a leaf
      nchild[pn] -= 1
      AMTup[pn].remove(i)          #   so its parent now has -1 child
      node_index_slice = node_index[xmin[i]:xmax[i]+1,ymin[i]:ymax[i]+1]
      node_index_slice[node_index_slice==i] = pn
    else:                                   # it's a non-leaf node
      c = AMTup[i]                        # get its children
      parent[c] = pn           #   and adjust its parent
      node_index_slice = node_index[xmin[i]:xmax[i]+1,ymin[i]:ymax[i]+1]
      node_index_slice[node_index_slice==i] = pn
      AMTup[pn].remove(i)                 # update the up links
      AMTup[pn].extend(AMTup[i])
      nchild[pn] += len(c) - 1 # update nchild
    parent[i] = i                # removed, point it to itself
  return


def contract_dr_3d_aux_py(children_list,cum_children_hist, to_keep,parent,\
                         nchild,xmin,xmax,ymin,ymax,zmin,zmax,node_index):

  AMTup =[]
  AMTup.append(children_list[0:cum_children_hist[0]].tolist())
  for i in xrange(1,parent.size):
    temp = children_list[cum_children_hist[i-1]:cum_children_hist[i]].tolist()
    AMTup.append(temp)


  to_remove = np.logical_not(to_keep)
  to_remove[0] = False

  for i in np.nonzero(to_remove)[0][::-1]:    # scanning from leaves to root
    pn = parent[i]                          # the node about to be removed has this parent
    if (nchild[i] == 0):                    # it is a leaf
      nchild[pn] -= 1
      AMTup[pn].remove(i)          #   so its parent now has -1 child
      node_index_slice = node_index[xmin[i]:xmax[i]+1,ymin[i]:ymax[i]+1,zmin[i]:zmax[i]+1]
      node_index_slice[node_index_slice==i] = pn
    else:                                   # it's a non-leaf node
      c = AMTup[i]                        # get its children
      parent[c] = pn           #   and adjust its parent
      node_index_slice = node_index[xmin[i]:xmax[i]+1,ymin[i]:ymax[i]+1,zmin[i]:zmax[i]+1]
      node_index_slice[node_index_slice==i] = pn
      AMTup[pn].remove(i)                 # update the up links
      AMTup[pn].extend(AMTup[i])
      nchild[pn] += len(c) - 1 # update nchild
    parent[i] = i                # removed, point it to itself
  return


def prune_aux_py(lut,to_prune, parent,nchild):
    N = parent.size
    for i in np.nonzero(to_prune)[0]:
        lut[i] = lut[parent[i]]
        nchild[parent[i]] -= 1
    return


def get_ancestors_aux_py(node, parent):
  dlist = [node]
  while parent[node] != node:
    dlist.append(parent[node])
    node = parent[node]
  return np.array(dlist, dtype = np.int32)


def get_children_aux_py(cum_children_hist,par):
    n = par.size
    children_list = np.zeros(n-1, dtype = np.int32)
    for i in xrange(1,n):
      cum_children_hist[par[i]] -=1
      children_list[cum_children_hist[par[i]]] = i;
    return children_list


def get_descendants_aux_py(node,children_list,cum_children_hist):
    descendants = [node]
    ndescendants = 1;
    index = 0;

    while (True):
        child_of_node = cum_children_hist[node - 1];
        nchild = cum_children_hist[node] - child_of_node;
        children_i = children_list[cum_children_hist[node - 1]:cum_children_hist[node]]
        for i in children_i:
            descendants.append(i)
        ndescendants += nchild;
        index += 1
        if (index == len(descendants)):
            break
        node = descendants[index]


    return np.array(descendants, dtype = np.int32)

def get_sub_branches_aux_py(parent,nchild,sb,visited):

    counter = 0
    nsb2 = 0
    cum_sb_hist = []
    for i in xrange(parent.size-1,0,-1):
        if (visited[i] == True):
            continue
        visited[i] = True
        cum_sb_hist.append(1)
        sb[counter] = i
        counter+=1
        par = parent[i]
        nchild_par = nchild[par]
        while(nchild_par == 1 and par!=0):
            sb[counter] = par
            counter+=1
            visited[par] = True
            par =parent[par]
            nchild_par = nchild[par]
            cum_sb_hist[nsb2]+=1

        nsb2+=1

    cum_sb_hist.append(1);
    return np.array(cum_sb_hist, dtype = np.int32).cumsum()



