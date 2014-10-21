#include <iostream>
#include <vector>


using namespace std;

#define MAXVALUE 255
#define RANGE 256
#define UNDEFINED -1

void counting_sort_c(int w1, unsigned char *flat_img,int **sorted_img, int *ww)
{
    int count[RANGE];
    int *sorted_img2 = new int[w1];
    *ww = w1;
    *sorted_img = sorted_img2;
    for(int i = 0; i < RANGE; i++) count[i] = 0;
    for(int i = 0; i < w1; i++) count[MAXVALUE - flat_img[i]]++; // histogram computation
    for(int i = 1; i < RANGE; i++) count[i] += count[i-1]; // Acumulated histogram
    for(int i = w1-1;  i>=0; i--)  sorted_img2 [--count[MAXVALUE - flat_img[i]]] = i;
}


inline int find_root(int *par, int p){
   if (par[p] != p)
      par[p] = find_root(par,par[p]);
   return par[p];
}


void canonicalize_c(int h_img, unsigned char *flat_img, int h_par, int *par, int h_S, int *S_rev){
   int q,p;
   for(int i = h_S-1; i > -1 ; i--){
      p = S_rev[i];
      q = par[p];
      if (flat_img[par[q]] == flat_img[q]) par[p] = par[q];
   }
}



void union_find2d_c(int H,int W, int h_off, int w_off, int *offsets, int h_par,
                  int *parent, int h_zpar, int *zpar, int h_S, int *S_rev,int h_img,
                  unsigned char *flat_img){

   int j = 0;
   int p, n;
   int zp, zn;
   int x,y;
   int x_n,y_n;
   int aux;

   for(int i = 0; i < h_S ; i++){
      p = S_rev[i];
      parent[p] = p;
      zpar[p] = p;
      zp = p;
      x = p/W;
      y = p%W;
      for(int k = 0; k < 2*h_off; k+=w_off){
         x_n = x + offsets[k];
         y_n = y + offsets[k + 1];
         n = x_n*W+y_n;
         if ((parent[n]!= UNDEFINED) && (x_n >= 0) &&  (x_n < H) &&  (y_n >= 0) &&
             (y_n < W)) {
            zn = find_root(zpar,n);
            if (zn!= zp){
               if (flat_img[zp] == flat_img[zn]){
                  aux = zn;
                  zn = zp;
                  zp = aux;
               }
               zpar[zn] = zp;
               parent[zn] = zp;
               S_rev[j] = zn;
               j++;
            }
         }
      }
   }
S_rev[h_S-1] = parent[S_rev[h_S-1]];
}

void computeNodeArray2d_c(int h_par, int *par, int h_img, unsigned char *flat_img,
                          int h_S, int *S_rev , int H, int W, int *node_index,
                          int **node_array, int *hh, int *ww){

   int k = 0;
   int nlevelroots = 0;
   int x,y;
   int pi,pindex,p;
   int raster,raster2;
   int ncols = 10;
   unsigned char level, plevel;

   int *area = new int[h_S];
   vector<int> sorted_lvroots;

   for(int i = 0; i < h_S; i++) area[i] = 1;
   for(int i = 0; i < h_S; i++){
      p = S_rev[i];
      area[par[p]] += area[p];
      if (flat_img[par[p]] != flat_img[p] || par[p] == p){
         sorted_lvroots.push_back (p);
         nlevelroots+=1;
      }
   }

   int *node_array2 = new int[ncols*nlevelroots];
   *hh = nlevelroots;
   *ww = ncols;
   *node_array = node_array2;


   for(int i = nlevelroots - 1; i >= 0; i--){
      p = sorted_lvroots[i];
      level = flat_img[p];
      plevel = flat_img[par[p]];
      node_index[p] = k;
      pindex = node_index[par[p]];
      node_array2[pindex*ncols + 1]+=1;
      raster = k*ncols;
      node_array2[raster] = pindex;      //  0 pindex
      node_array2[++raster] = 0;           //  1 isleaf
      node_array2[++raster] = level;       //  2 level
      node_array2[++raster] = area[p];    //  3 area
      node_array2[++raster] = 0;          //  4 sumx
      node_array2[++raster] = 10000;          //  5 xmin
      node_array2[++raster] = 0;      //  6 xmax
      node_array2[++raster] = 0;      //  7 sumy
      node_array2[++raster] = 10000;          //  8 ymin
      node_array2[++raster] = 0;         //  9 ymax
      k++;
   }

   for(int i = 0; i < h_S; i++){
      if(node_index[i] == UNDEFINED) node_index[i] = node_index[par[i]];
      k = node_index[i];
      y = i%W;
      x = i/W;
      raster = k*ncols + 4;
      node_array2[raster] += x;
      if (x < node_array2[++raster]) node_array2[raster] = x;
      if (x > node_array2[++raster]) node_array2[raster] = x;
      node_array2[++raster] += y;
      if (y < node_array2[++raster]) node_array2[raster] = y;
      if (y > node_array2[++raster]) node_array2[raster] = y;
   }

   for(int i = nlevelroots -1; i >=0 ; i--){
      raster = i*ncols;
      pi = node_array2[raster];
      raster2 = pi*ncols;
      node_array2[raster2 + 4] += node_array2[raster + 4];
      if(node_array2[raster + 5] < node_array2[raster2 + 5])
         node_array2[raster2 + 5] = node_array2[raster + 5];
      if(node_array2[raster + 6] > node_array2[raster2 + 6])
         node_array2[raster2 + 6] = node_array2[raster + 6];
      node_array2[raster2 + 7] += node_array2[raster + 7];
      if(node_array2[raster + 8] < node_array2[raster2 + 8])
         node_array2[raster2 + 8] = node_array2[raster + 8];
      if(node_array2[raster + 9] > node_array2[raster2 + 9])
         node_array2[raster2 + 9] = node_array2[raster + 9];
   }
   node_array2[3] = node_array2[3]/2;
   node_array2[4] = node_array2[4]/2;
   node_array2[5] = node_array2[5]/2;

   delete[] area;
}


// Union-find implementation with level compression
void union_find3d_c(int L,int M, int N, int h_off, int w_off, int *offsets, int h_par,
                  int *parent, int h_zpar, int *zpar, int h_S, int *S_rev,
                  int h_img, unsigned char *flat_img){

   int j;
   int p; //pixel being processed;
   int n; // neighbor of the pixel beinf processed
   int x,y,z; // coordinates of the pixel being processed
   int x_n,y_n,z_n; // coordinates of the neighbor of thepixel being processed
   int zp,zn;
   int aux;
   int MN = M*N;
   j = 0;

   for(int i = 0; i < h_S; i++){
      p = S_rev[i];
      parent[p] = p;
      zpar[p] = p;
      zp = p;
      x = p/MN;
      aux = (p-x*MN);
      y = aux/N;
      z = (aux)%N;

      for(int k = 0; k < w_off*h_off; k+=w_off){
         x_n = x + offsets[k];
         y_n = y + offsets[k + 1];
         z_n = z + offsets[k + 2];
         if ((x_n >= 0) &&  (x_n < L) &&  (y_n >= 0) &&  (y_n < M) &&
             (z_n >= 0) &&  (z_n < N)) {
            n = x_n*MN + y_n*N +z_n;
            if (parent[n]!= UNDEFINED){
               zn = find_root(zpar,n);
               if (zn!= zp){
                  if (flat_img[zp] == flat_img[zn]){
                     aux = zn;
                     zn = zp;
                     zp = aux;
                  }
                  zpar[zn] = zp;
                  parent[zn] = zp;
                  S_rev[j] = zn;
                  j++;
               }
            }
         }
      }
   }
S_rev[h_S-1] = parent[S_rev[h_S-1]];
}

void computeNodeArray3d_c(int h_par, int *par, int h_img, unsigned char *flat_img,
                          int h_SR, int *S_rev , int L, int M, int N, int *node_index,
                          int **node_array, int *hh, int *ww){

   int x,y,z,pi,pindex,p,temp;
   int MN = M*N;
   int k = 0;
   int nlevelroots = 0;
   int n = L*MN;
   int *area = new int[n];
   int raster,raster2;
   int ncols = 13;

   unsigned char level, plevel;
   vector<int> sorted_lvroots;


   for(int i = 0; i < n; i++) area[i] = 1;
   for(int i = 0; i < n; i++){
      p = S_rev[i];
      area[par[p]] += area[p];
      if (flat_img[par[p]] != flat_img[p] || par[p] == p){
         sorted_lvroots.push_back (p);
         nlevelroots+=1;
      }
   }

   int *node_array2 = new int[ncols*nlevelroots];
   *hh = nlevelroots;
   *ww = ncols;
   *node_array = node_array2;


   for(int i = nlevelroots - 1; i >= 0; i--){
      p = sorted_lvroots[i];
      level = flat_img[p];
      plevel = flat_img[par[p]];
      node_index[p] = k;
      pindex = node_index[par[p]];
      node_array2[pindex*ncols + 1]+=1;
      raster = k*ncols;
      node_array2[raster] = pindex;      //  0 pindex
      node_array2[++raster] = 0;           //  1 isleaf
      node_array2[++raster] = level;       //  2 level
      node_array2[++raster] = area[p];    //  3 area
      node_array2[++raster] = 0;          //  4 sumx
      node_array2[++raster] = 10000;          //  5 xmin
      node_array2[++raster] = 0;      //  6 xmax
      node_array2[++raster] = 0;      //  7 sumy
      node_array2[++raster] = 10000;          //  8 ymin
      node_array2[++raster] = 0;         //  9 ymax
      node_array2[++raster] = 0;      //  10 sumz
      node_array2[++raster] = 10000;          //  11 zmin
      node_array2[++raster] = 0;         //  12 zmax
      k++;
   }

   for(int i = 0; i < n; i++){
      if(node_index[i] == UNDEFINED) node_index[i] = node_index[par[i]];
      k = node_index[i];
      x = i/MN;
      temp = (i - x*MN);
      y = temp/N;
      z = temp%N;
      raster = k*ncols + 4;
      node_array2[raster] += x;
      if (x < node_array2[++raster]) node_array2[raster] = x;
      if (x > node_array2[++raster]) node_array2[raster] = x;
      node_array2[++raster] += y;
      if (y < node_array2[++raster]) node_array2[raster] = y;
      if (y > node_array2[++raster]) node_array2[raster] = y;
      node_array2[++raster] += z;
      if (z < node_array2[++raster]) node_array2[raster] = z;
      if (z > node_array2[++raster]) node_array2[raster] = z;
   }

   for(int i = nlevelroots -1; i >=0 ; i--){
      raster = i*ncols;
      pi = node_array2[raster];
      raster2 = pi*ncols;
      node_array2[raster2 + 4] += node_array2[raster + 4];
      if(node_array2[raster + 5] < node_array2[raster2 + 5])
         node_array2[raster2 + 5] = node_array2[raster + 5];
      if(node_array2[raster + 6] > node_array2[raster2 + 6])
         node_array2[raster2 + 6] = node_array2[raster + 6];
      node_array2[raster2 + 7] += node_array2[raster + 7];
      if(node_array2[raster + 8] < node_array2[raster2 + 8])
         node_array2[raster2 + 8] = node_array2[raster + 8];
      if(node_array2[raster + 9] > node_array2[raster2 + 9])
         node_array2[raster2 + 9] = node_array2[raster + 9];
      node_array2[raster2 + 10] += node_array2[raster + 10];
      if(node_array2[raster + 11] < node_array2[raster2 + 11])
         node_array2[raster2 + 11] = node_array2[raster + 11];
      if(node_array2[raster + 12] > node_array2[raster2 + 12])
         node_array2[raster2 + 12] = node_array2[raster + 12];
   }
   node_array2[3] = node_array2[3]/2;
   node_array2[4] = node_array2[4]/2;
   node_array2[7] = node_array2[7]/2;
   node_array2[10] = node_array2[10]/2;
   delete[] area;
}
