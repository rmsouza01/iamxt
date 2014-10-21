#include <iostream>
#include <vector>
#include <math.h>

using namespace std;

inline int mymaximum(int val1, int val2){
   if (val1 >= val2)
      return val1;
   else
      return val2;
}

void compute_height_aux_c(int h1, int *par, int h2, int *delta, int h3, int *height){
   int parent_i;
   for(int i = (h1-1); i > 0; i--){
      parent_i = par[i];
      height[parent_i] = mymaximum(height[parent_i], (height[i] + delta[i]) );
   }
}

void compute_volume_aux_c(int h1, int *par, int h2, int *delta, int h3, int *area, int h4, int *volume){
   int parent_i;

   for(int i = (h1-1); i > 0; i--){
      volume[par[i]] += volume[i] + (delta[i] - 1)*area[i];
      }
   }

void compute_extinction_values_aux_c(int h1, int *par, int h2, int *attrib, int h3,
                                     int *leaves,int h4, int *ichmax, int h5,
                                     int *achmax, int h6, int *ext_values){
   int vv,big,par_i, node,nd;
   for(int i = (h1 -1); i>0; i--){
      par_i = par[i];
      if (attrib[i] > achmax[par_i]){
         achmax[par_i] = attrib[i];
         ichmax[par_i] = i;
      }
   }

   for(int i = 0; i < h3; i++){
      node = nd = leaves[i];
      while(par[node] != node){
         vv = attrib[node];
         big = ichmax[par[node]];
         if (node != big){
            ext_values[nd] = vv;
            break;
         }
         node = par[node];
      }
      if (par[node] == node)
         ext_values[nd] = achmax[node];

   }
}


void compute_stability_measure_aux_c(int h_par, int *par, int h_level, int *level, int h_area,
                                     int *area, int h2, int *nlevels,int h3, int *nodes_list,
                                     int h4, double *stability_measure, int delta, int hmin) {
      int att0,att1,node, ancestor_index, current_level;
      for(int i = 0; i < h3; i++){
         node = nodes_list[i];
         current_level = level[node];
         if (nlevels[node] > delta) ancestor_index = node;
         else{
            ancestor_index = par[node];
            while (1){
               if ((current_level - level[ancestor_index]+ nlevels[ancestor_index]) >delta)
                  break;
               else
                  ancestor_index = par[ancestor_index];
               }
            }
         att0 = area[ancestor_index];
         att1 = area[node];
         stability_measure[node] = 1.0*(att0- att1)/att0;
      }
   }

void get_signature_aux_c(int h1, int *par, int h2, int *h, int h3, int *area,
                         int h4, int *attrib, int **signature, int *h5,
                         int start, int end, int cte){
   *h5 = h[start] - h[end] + 1;
   int *signature2 = new int[*h5];
   *signature = signature2;
   int par_i,node,nlevels, counter = *h5 - 1;
   node = start;

   if (cte == 0){
      while (node!= end){
         nlevels = h[node] - h[par[node]];
         for( int i = 0; i < nlevels; i++){
            signature2[counter] = attrib[node];
            counter--;
         }
         node = par[node];
      }
   }
  else if (cte == 1){
      while (node != end){
         nlevels = h[node] - h[par[node]];
         for( int i = 0; i < nlevels; i++){
            signature2[counter] = attrib[node] + i*area[node];
            counter--;
         }
         node = par[node];
      }
   }

   signature2[counter] = attrib[node];
}

void extinction_filter_aux_c(int h1, int *to_remove, int h_ii, int *ii, int h_par, int *par){

   int node, par_node;
   to_remove[0] = 0;
   for (int j = 0; j < h_ii; j++){
      node = ii[j];
      par_node = par[node];
      while(to_remove[node]!= 0){
         to_remove[node] = 0;
         node = par_node;
         par_node = par[node];
      }
   }
}

void mms_mser_aux_c(int h1, double *stability_measure,int h2, int *to_keep,int h3,
                    int *sub_branches,int h4, int *sb_cum_hist){
   int node,next_node;
   int j = 0;
   double stability, next_stability;

   for(int i = 0; i < h4-2; i++){
      node = sub_branches[sb_cum_hist[i]];
      stability = stability_measure[node];
      for(int k = (sb_cum_hist[i] + 1); k < sb_cum_hist[i+1]; k++){
         next_node = sub_branches[k];
         next_stability = stability_measure[next_node];
         if (next_stability < stability){
            node = next_node;
            stability = next_stability;
            }
         }
      to_keep[j] = node;
      j++;
         }
   }


void mms_t_aux_c(double t, int h1, int *nlevels, int h_level, int *level, int h2, int *to_keep, int h3, int *new_h,
             int h4, int *sub_branches,int h5, int *sb_cum_hist){
   int j = 0;
   int ngreylevels = 0;
   int h_sb,node, start_node;
   int ww, kk;
   double h_new;

   for(int i = 0; i < h5-2; i++){
      start_node = sub_branches[sb_cum_hist[i+1] -1];
      for(int k = sb_cum_hist[i]; k < sb_cum_hist[i+1]; k++){
         node = sub_branches[k];
         ngreylevels += nlevels[node];
         }
      ngreylevels--;
      //h_new = level[start_node] - nlevels[start_node] + 1 + t*(ngreylevels - 1);
      h_new = level[start_node] - nlevels[start_node] + rint(1 + t*ngreylevels);
      //new_h[j] = round(h_new);
      new_h[j] = h_new;

      ww = sb_cum_hist[i];
      kk = sub_branches[ww];
      h_sb = level[kk];

      while(! ((h_sb - nlevels[kk]) < h_new && (h_new<= h_sb))){
         ww++;
         kk = sub_branches[ww];
         h_sb = level[kk];
         }
      to_keep[j] = kk;
      j++;
      ngreylevels = 0;
      }
   }


