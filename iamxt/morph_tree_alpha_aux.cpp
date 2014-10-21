#include <iostream>
#include <vector>

using namespace std;


void get_descendants_aux_c(int node, int h1, int *children_list, int h2, int *cum_children_hist, int **descendants, int *h3){
   vector<int> descendants_vector;
   int child_of_node,nchild,ndescendants,index;
   descendants_vector.push_back(node);
   ndescendants = 1;
   index = 0;
   while (1){

      child_of_node = cum_children_hist[node - 1];
      nchild = cum_children_hist[node] - child_of_node;

      for(int i = 0; i< nchild; i++) descendants_vector.push_back(children_list[child_of_node++]);
      ndescendants += nchild;
      index++;
      if (index == descendants_vector.size()) break;
      node = descendants_vector[index];


   }
   *h3 = ndescendants;
   int *descendants2 = new int[ndescendants];
   *descendants = descendants2;
   for(int i = 0; i < ndescendants; i++) descendants2[i] = descendants_vector[i];
}

void get_ancestors_aux_c(int node, int h_par, int  *par , int **ancestors, int *hh){
   int current_node = node;
   vector<int> ancestors2;
   ancestors2.push_back (current_node);
   while(current_node!= 0){
      current_node = par[current_node];
      ancestors2.push_back(current_node);
   }
   *hh = ancestors2.size();
   int *temp = new int[*hh];
   for(int i = 0; i< *hh; i++) temp[i] = ancestors2[i];
   *ancestors = temp;
}

void get_children_aux_c(int n, int  *cum_hist, int n2, int  *par , int **children_list, int *hh){
    *hh = n-1;
    int *children_list2 = new int[n-1];
    *children_list = children_list2;
    for(int i = 1; i < n; i++) children_list2[--cum_hist[par[i]]] = i;
}

void get_sub_branches_aux_c(int h_par,int *par, int h_nchild, int *nchild,int h_sb,
                            int *sb,int h_v,int *visited,int **sb_cum_hist,int *nsb){
   vector<int> sb_cum_hist2;
   int par_i,nchild_i, index;
   int counter = 0;
   int nsb2 = 0;

   for (int i = h_par -1; i > 0; i--){
      if (visited[i] == 1) continue;
      visited[i] = 1;
      sb_cum_hist2.push_back(1);
      sb[counter] = i;
      counter++;
      par_i = par[i];
      nchild_i = nchild[par_i];
      while(nchild_i == 1 && par_i!=0){
         sb[counter] = par_i;
         counter++;
         visited[par_i] = 1;
         par_i = par[par_i];
         nchild_i = nchild[par_i];
         sb_cum_hist2[nsb2]++;
         }
      nsb2++;
      }
   *nsb = nsb2+1;
   sb_cum_hist2.push_back(1);
   int *temp = new int[*nsb];
   temp[0] = sb_cum_hist2[0];
   for(int i = 1; i < *nsb; i++) temp[i] = temp[i-1] + sb_cum_hist2[i];
   *sb_cum_hist = temp;
}

inline void update_node_index2d_c(int new_value, int old_value, int w,
                                  int *node_index, int bbox[]){
   int p;
   for(int i = bbox[0]; i <= bbox[1]; i++){
      p = i*w +  bbox[2];
      for(int j = 0; j <= (bbox[3] - bbox[2]); j++){
         if (node_index[p] == old_value) node_index[p] = new_value;
         p++;
      }
   }
}


inline void update_node_index3d_c(int new_value, int old_value, int M,
                                  int N, int *node_index, int bbox[]){
   int p;
   int aux,aux2;
   aux = M*N;
   for(int i = bbox[0]; i <= bbox[1]; i++){
      aux2 = i*aux;
      for(int j = bbox[2]; j <= bbox[3]; j++){
         p = aux2 + j*N + bbox[4];
         for(int k = 0; k <= (bbox[5]- bbox[4]); k++){
             if (node_index[p] == old_value) node_index[p] = new_value;
             p++;
         }
      }
   }
}


void contract_dr_2d_aux_c(int h1, int *children_list, int h2, int *cum_children_hist,
                          int h3, int *to_keep, int h_par, int  *par , int h_nchild,
                          int  *nchild,int h_xmin, int  *xmin,int h_xmax, int  *xmax,
                          int h_ymin, int  *ymin,int h_ymax, int  *ymax, int h_ni, int w_ni,
                          int *node_index){

   int temp, temp_val, aux,par_i, temp2;
   vector<int> nodes_line;
   int bbox[4];

   // Computation of the nearest ancestor node kept for
   // each node of the max-tree
   int *nearest_ancestor_kept = new int[h_par];
   nearest_ancestor_kept[0] = 0;

   for(int i = 1; i < h_par; i++){
      if (to_keep[i] == 0)
         nearest_ancestor_kept[i] = nearest_ancestor_kept[par[i]];
      else
         nearest_ancestor_kept[i] = i;
   }

   //root
   for (int j = 0; j < cum_children_hist[0];j++){
      if (to_keep[children_list[j]] != 1)
         nchild[0]--;
      }

   for(int i = h_par-1; i > 0; i--){
      if (to_keep[i] == 1  ){
         par_i = par[i];
         if (to_keep[par_i] != 1){
            par[i] = nearest_ancestor_kept[par_i];
            nchild[nearest_ancestor_kept[par_i]]++;
            }
         for (int j = cum_children_hist[i-1]; j < cum_children_hist[i];j++){
            if (to_keep[children_list[j]] != 1)
               nchild[i]--;
            }
         }
      else if (to_keep[i] == 0) {

         nodes_line.clear();
         temp = i;
         temp_val = to_keep[temp];
         // Compute the sequence of nodes to be removed
         while (temp_val == 0){
            to_keep[temp] = 2; //
            nodes_line.push_back(temp);
            temp = par[temp];
            temp_val = to_keep[temp];
            }

         // Updates node_index
         for (int j = 0; j < nodes_line.size(); j++){
            temp2 = nodes_line[j];
            bbox[0] = xmin[temp2];
            bbox[1] = xmax[temp2];
            bbox[2] = ymin[temp2];
            bbox[3] = ymax[temp2];
            update_node_index2d_c(nearest_ancestor_kept[temp],  temp2, w_ni, node_index, bbox);
            }
         }
      }
   for(int i = 1; i < h_par; i++){
      if (to_keep[i] != 1)
         par[i] = i;
      }
   delete[] nearest_ancestor_kept;
}


void contract_dr_3d_aux_c(int h1, int *children_list, int h2, int *cum_children_hist, int h3, int *to_keep,
int h_par, int  *par , int h_nchild, int  *nchild,int h_xmin, int  *xmin,int h_xmax, int  *xmax,
int h_ymin, int  *ymin, int h_ymax, int  *ymax, int h_zmin, int  *zmin, int h_zmax, int  *zmax,
int L_ni, int M_ni, int N_ni, int *node_index){

   int temp, temp_val, aux,par_i, temp2;
   vector<int> nodes_line;
   int bbox[4];

   // Computation of the nearest ancestor node kept for
   // each node of the max-tree
   int *nearest_ancestor_kept = new int[h_par];
   nearest_ancestor_kept[0] = 0;

   for(int i = 1; i < h_par; i++){
      if (to_keep[i] == 0)
         nearest_ancestor_kept[i] = nearest_ancestor_kept[par[i]];
      else
         nearest_ancestor_kept[i] = i;
   }

   //root
   for (int j = 0; j < cum_children_hist[0];j++){
      if (to_keep[children_list[j]] != 1)
         nchild[0]--;
      }

   for(int i = h_par-1; i > 0; i--){
      if (to_keep[i] == 1  ){
         par_i = par[i];
         if (to_keep[par_i] != 1){
            par[i] = nearest_ancestor_kept[par_i];
            nchild[nearest_ancestor_kept[par_i]]++;
            }
         for (int j = cum_children_hist[i-1]; j < cum_children_hist[i];j++){
            if (to_keep[children_list[j]] != 1)
               nchild[i]--;
            }
         }
      else if (to_keep[i] == 0) {

         nodes_line.clear();
         temp = i;
         temp_val = to_keep[temp];
         // Compute the sequence of nodes to be removed
         while (temp_val == 0){
            to_keep[temp] = 2; //
            nodes_line.push_back(temp);
            temp = par[temp];
            temp_val = to_keep[temp];
            }

         // Updates node_index
         for (int j = 0; j < nodes_line.size(); j++){
            temp2 = nodes_line[j];
            bbox[0] = xmin[temp2];
            bbox[1] = xmax[temp2];
            bbox[2] = ymin[temp2];
            bbox[3] = ymax[temp2];
            bbox[4] = zmin[temp2];
            bbox[5] = zmax[temp2];
            update_node_index3d_c(nearest_ancestor_kept[temp],  temp2, M_ni,N_ni, node_index, bbox);
            }
         }
      }
   for(int i = 1; i < h_par; i++){
      if (to_keep[i] != 1)
         par[i] = i;
      }
   delete[] nearest_ancestor_kept;
}


void prune_aux_c(int h1, int *lut, int h2, int *to_prune, int h_par, int *par,
                 int h_nchild, int *nchild){
   int par_i;
   for (int i = 0; i < h1; i++){
      if (to_prune[i] == 1){
         par_i = par[i];
         lut[i] = lut[par_i];
         nchild[par_i]--;
      }
   }
}
