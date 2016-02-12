%module watershed_c 
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

void ws_markers_c(int, int ,int , int *, int ,int , int , unsigned short *, int ,int , int , int *,
                  int ,int , int , int *, int ,int , int , int *, int ,int , int , int *,
                  int ,int , int , int *);

void tz_ws_c(int , int ,int , int *, int ,int , int , unsigned short *, int ,int , int , int *,
             int  ,int , int , int *,int  ,int , int , int *,int  ,int , int , int *,
             int  ,int , int , int *);

%}

%feature("autodoc", 1);


void ws_markers_c(int ii32,int DIM1,int DIM2, int *INPLACE_ARRAY2,
                  int DIM1,int DIM2, int DIM3, unsigned short *INPLACE_ARRAY3,
                  int DIM1,int DIM2, int DIM3, int *INPLACE_ARRAY3,
                  int DIM1,int DIM2, int DIM3, int *INPLACE_ARRAY3,
                  int DIM1,int DIM2, int DIM3, int *INPLACE_ARRAY3,
                  int DIM1,int DIM2, int DIM3, int *INPLACE_ARRAY3,
                  int DIM1,int DIM2, int DIM3, int *INPLACE_ARRAY3);


void tz_ws_c(int ii32, int DIM1,int DIM2, int *INPLACE_ARRAY2,
             int DIM1,int DIM2, int DIM3, unsigned short *INPLACE_ARRAY3,
             int DIM1,int DIM2, int DIM3, int *INPLACE_ARRAY3,
             int DIM1,int DIM2, int DIM3, int *INPLACE_ARRAY3,
             int DIM1,int DIM2, int DIM3, int *INPLACE_ARRAY3,
             int DIM1,int DIM2, int DIM3, int *INPLACE_ARRAY3,
             int DIM1,int DIM2, int DIM3, int *INPLACE_ARRAY3);
