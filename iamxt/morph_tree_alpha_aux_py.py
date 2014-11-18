# -*- encoding: utf-8 -*-

import numpy as np

def contract_dr_aux_py(to_keep,lut,par):
        
    nearest_ancestor_kept = np.zeros(par.size) 
       
    for i in xrange(1,par.size):
        if (not to_keep[i]):
            temp = nearest_ancestor_kept[par[i]]; 
            nearest_ancestor_kept[i] = temp
            lut[i] = lut[temp];
             
        else:
            nearest_ancestor_kept[i] = i; 
            par[i] = nearest_ancestor_kept[par[i]];
    return
 
def update_nchild_aux_py(par,nchild):
    for i in xrange(1,par.size):
        nchild[par[i]] += 1   


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



