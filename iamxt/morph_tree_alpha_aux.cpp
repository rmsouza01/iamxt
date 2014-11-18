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

    void contract_dr_aux_c(int h1,  int *to_keep,int h_lut, int *lut, int h_par,int  *par){
     
       int temp;
       
       
       // Computation of the nearest ancestor node kept for 
       // each node of the max-tree
       int *nearest_ancestor_kept = new int[h_par]; 
       nearest_ancestor_kept[0] = 0;
       
       for(int i = 1; i < h_par; i++){
          if (to_keep[i] == 0){
             temp = nearest_ancestor_kept[par[i]]; 
             nearest_ancestor_kept[i] = temp;
             lut[i] = lut[temp];
             }
          else {
             nearest_ancestor_kept[i] = i; 
             par[i] = nearest_ancestor_kept[par[i]];
             } 
       }
       
       delete[] nearest_ancestor_kept;
    }   

    void update_nchild_aux_c(int h1,  int *par,int h2, int *nchild){
     
       for(int i = 1; i < h1; i++) nchild[par[i]]++;
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
