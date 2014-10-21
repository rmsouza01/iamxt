# -*- encoding: utf-8 -*-
# Module morph_tree_alpha

from build_max_tree import build_max_tree
from morph_tree_alpha_methods import getImage, clone, recConnectedComponent, compact,\
				     areaOpen, bbox, computeRR, generateCCPathGraph, \
                                     generateCCGraph, generateGraph, getChildren, getAncestors, \
                                     getDescendants, getSubBranches, prune, contractDR



from morph_tree_alpha_aux import get_children_aux_c, get_ancestors_aux_c, get_descendants_aux_c, get_sub_branches_aux_c,\
                                 prune_aux_c, contract_dr_2d_aux_c, contract_dr_3d_aux_c

from morph_tree_alpha_aux_py import get_children_aux_py, get_ancestors_aux_py, get_descendants_aux_py,\
                                    get_sub_branches_aux_py, prune_aux_py, contract_dr_2d_aux_py, contract_dr_3d_aux_py



class MorphTreeAlpha:
    """
    This class builds the morphological tree corresponding to a 8-bit
    grayscale image. The morphological trees available for
    construction are 2D and 3D max-trees, and 2D tree of shapes.
    **Input:**
    img -> uint8 image, may be either 2D or 3D. When working with 1D
    signals use a 2D array with the shape 1xW.
    Bc -> Boolean array corresponding to the connectivity to be used
    during the tree construction. The convention is that coordinates
    (0,0) or (0,0,0) are in the center of the array.
    option-> string, it may either be 'max_tree' or 'tree_of_shapes'
    """


    getImage = getImage
    clone = clone
    recConnectedComponent = recConnectedComponent
    computeRR = computeRR
    areaOpen = areaOpen
    bbox = bbox
    compact = compact
    generateCCPathGraph = generateCCPathGraph
    generateCCGraph = generateCCGraph
    generateGraph = generateGraph
    getChildren = getChildren
    getAncestors = getAncestors
    getDescendants = getDescendants
    getSubBranches = getSubBranches
    prune = prune
    contractDR = contractDR


    def __init__(self,img = None, Bc = None,option = 'max_tree', implementation = 'c'):
        if option == 'max_tree':
            _,_,self.node_array,self.node_index, = build_max_tree(img,Bc, option = 1)
        elif option == 'tree_of_shapes':
            print "Error: Option not implemented yet"
        else:
            print "Error: invalid option"
            return
        self.Bc = Bc
        self.shape = img.shape
        self._children_list = []
        self._cum_children_hist = []
        self._children_updated = False
        self._sb = []
        self._cum_sb_hist = []
        self._sb_updated = False
        self.implementation = implementation



        if self.implementation == 'c':
            self.get_children_aux = get_children_aux_c
            self.get_ancestors_aux = get_ancestors_aux_c
            self.get_descendants_aux = get_descendants_aux_c
            self.get_sub_branches_aux = get_sub_branches_aux_c
            self.prune_aux = prune_aux_c
            self.contract_dr_2d_aux = contract_dr_2d_aux_c
            self.contract_dr_3d_aux = contract_dr_3d_aux_c
        else:
            self.get_children_aux =  get_children_aux_py
            self.get_ancestors_aux = get_ancestors_aux_py
            self.get_descendants_aux = get_descendants_aux_py
            self.get_sub_branches_aux = get_sub_branches_aux_py
            self.prune_aux = prune_aux_py
            self.contract_dr_2d_aux = contract_dr_2d_aux_py
            self.contract_dr_3d_aux = contract_dr_3d_aux_py

