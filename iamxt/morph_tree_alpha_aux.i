%module morph_tree_alpha_aux
%{
#define SWIG_FILE_WITH_INIT
#undef NO_IMPORT_ARRAY
%}

%include "typemaps.i"
%include "numpy.i"

%init %{
import_array();
%}

%{

void get_descendants_aux_c(int node, int , int *, int , int *, int **, int *);

void get_ancestors_aux_c(int node, int , int *,int **, int *);

void get_children_aux_c(int ,  int *,int ,  int *, int **, int *);

void get_sub_branches_aux_c(int , int *, int , int *,
                            int , int *,int , int *,
                            int **, int *);

void contract_dr_aux_c(int,  int *,int, int *, int,int  *);

void update_nchild_aux_c(int,  int *,int, int *);

void prune_aux_c(int , int *, int , int *, int ,
                 int *, int , int *);

%}

%feature("autodoc", 1);
void get_descendants_aux_c(int node, int DIM1, int *IN_ARRAY1, int DIM1, int *IN_ARRAY1, int **ARGOUT_ARRAY1, int *DIM1);

void get_ancestors_aux_c(int node, int DIM1, int *INPLACE_ARRAY1,int **ARGOUT_ARRAY1, int *DIM1);

void get_children_aux_c(int DIM1,  int *IN_ARRAY1,int DIM1,  int *INPLACE_ARRAY1, int **ARGOUT_ARRAY1, int *DIM1);

void get_sub_branches_aux_c(int DIM1, int *INPLACE_ARRAY1, int DIM1, int *INPLACE_ARRAY1,
                            int DIM1, int *INPLACE_ARRAY1,int DIM1, int *INPLACE_ARRAY1,
                            int **ARGOUT_ARRAY1, int *DIM1);

void contract_dr_aux_c(int DIM1, int *INPLACE_ARRAY1, int DIM1, int *INPLACE_ARRAY1,
                           int DIM1,int *INPLACE_ARRAY1);

void update_nchild_aux_c(int DIM1, int *INPLACE_ARRAY1, int DIM1, int *INPLACE_ARRAY1);

void prune_aux_c(int DIM1, int *INPLACE_ARRAY1, int DIM1, int *INPLACE_ARRAY1, int DIM1,
                 int *INPLACE_ARRAY1, int DIM1, int *INPLACE_ARRAY1);

