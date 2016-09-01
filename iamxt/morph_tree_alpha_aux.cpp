//BSD 2-Clause License
//Copyright (c) 2016, Roberto Souza and collaborators
//All rights reserved. 


#include <iostream>
#include <vector>

#ifdef _OPENMP
   #include <omp.h>
#else
   #define omp_get_thread_num() 0
#endif

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
        if (index == static_cast<int>(descendants_vector.size())) break;
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
    int par_i,nchild_i;
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

void get_image_aux_2d_c(int h1, int *h,int h2,int w2,int *node_index,int h3,int w3,unsigned short *output_img) {
    
    int N =  h2*w2;
#pragma omp parallel for shared(output_img,h,N,node_index)
    for(int i = 0; i < N;i++ ){
        output_img[i] = h[node_index[i]];
    }
    
}

void get_image_aux_3d_c(int h1, int *h,int h2,int w2, int z2,int *node_index,int h3,int w3,int z3, unsigned short *output_img) {
    
    int N =  h2*w2*z2;
#pragma omp parallel for shared(output_img,h,N,node_index)
    for(int i = 0; i < N;i++ ){
        output_img[i] = h[node_index[i]];
    }
    
}

void rec_connected_component_2d_c(int node, int seed, int h_ni,int w_ni,
                                  int *NI,int h_cc,int w_cc, unsigned char *cc,
                                  int h_off, int w_off,int *offsets){
    
    vector<int> seeds_queue;
    seeds_queue.push_back (seed);
    int p,n,x_n, y_n;
    cc[seed] = 1;
    
    while ( !seeds_queue.empty() ){
        p = seeds_queue.back();
        seeds_queue.pop_back();
        
        for(int k = 0; k < w_off*h_off; k+=w_off){
            x_n = p/w_ni + offsets[k];
            y_n = p%w_ni + offsets[k + 1];
            n = x_n*w_ni + y_n;
            if ((x_n >= 0) &&  (x_n < h_ni) &&
                (y_n >= 0) &&  (y_n < w_ni) &&
                (NI[n] >= node) && cc[n]!=1 ){
                seeds_queue.push_back(n);
                cc[n] = 1;
            }
        }
    }
}

void rec_connected_component_3d_c(int node, int seed, int h_ni,int w_ni,int z_ni,
                                  int *NI,int h_cc,int w_cc,int z_cc, unsigned char
                                  *cc,int h_off, int w_off,int *offsets){
    
    vector<int> seeds_queue;
    seeds_queue.push_back (seed);
    int p, n, x, y, z, aux, x_n, y_n, z_n;
    int MN = w_ni*z_ni;
    cc[seed] = 1;
    
    while ( !seeds_queue.empty() ){
        p = seeds_queue.back();
        seeds_queue.pop_back();
        x = p/MN;
        aux = (p-x*MN);
        y = aux/z_ni;
        z = (aux)%z_ni;
        for(int k = 0; k < w_off*h_off; k+=w_off){
            x_n = x + offsets[k];
            y_n = y + offsets[k + 1];
            z_n = z + offsets[k + 2];
            
            
            n = x_n*MN + y_n*z_ni + z_n;
            if ((x_n >= 0) &&  (x_n < h_ni) &&
                (y_n >= 0) &&  (y_n < w_ni) &&
                (z_n >= 0) &&  (z_n < z_ni) &&
                (NI[n] >= node) && cc[n]!=1 ){
                seeds_queue.push_back(n);
                cc[n] = 1;
            }
        }
    }
}

void lut_node_index_2d_c(int h, int *lut, int h1, int w1, int *node_index){
    int N = h1*w1;
    
#pragma omp parallel for shared(N,lut,node_index)
    for(int i = 0; i < N ;i++ ){
        node_index[i] = lut[node_index[i]];
        
    }
    
}

void lut_node_index_3d_c(int h, int *lut, int h1, int w1,int z1, int *node_index){
    int N = h1*w1*z1;
    
#pragma omp parallel for shared(N,lut,node_index)
    for(int i = 0; i < N ;i++ ){
        node_index[i] = lut[node_index[i]];
        
    }
    
}

void remove_node_array_lines_c(int h1, int *nodes_kept,int h2,int w2,int *new_node_array,
                               int h_na, int w_na, int *node_array){
#pragma omp parallel for
    for(int i = 0; i< h1;i++){
        int index1,index2;
        index1 = i;
        index2 = nodes_kept[i];
        for (int j = 0; j< h2;j++){
            new_node_array[index1] = node_array[index2];
            index1+=w2;
            index2+=w_na;
        }
    }
}


int get_bif_ancestor_aux_c(int node, int h_par, int  *par ,int h_nchild, int  *nchild ){
    while(node!= 0 && nchild[par[node]] == 1 ){
        node = par[node];
    }
    return node;
}


void compute_hist_aux_c(int h_par, int *par, int h_hist, int w_hist, int *hist){
    int par_i, index1,index2;
    for (int i = h_par-1; i>0;i--){
        par_i = par[i];
        index1 = i*w_hist;
        index2 = par_i*w_hist;
        
        for(int j = 0; j < w_hist; j++){
            hist[index2 + j] +=  hist[index1 + j];
        }
    }
}


void compute_node_gray_avg_aux_c(int h1, int *par, int h2, int *h, int h3,
                                 int *area,int h4, double *gray_avg){
    
    for (int i=1; i < h1;i++){
        area[par[i]]-=area[i];
    }
    for (int i=0; i < h1;i++){
        gray_avg[i]=area[i]*h[i];
    }
    
    for (int i=h1-1; i > 0;i--){
        gray_avg[par[i]] += gray_avg[i];
    }
    
}


void compute_node_gray_var_aux_c(int h1, int *par, int h2, int *h, int h3,
                                 int *area,int h4, double *squared_gray_avg){
    
    int aux;
    
    for (int i=1; i < h1;i++){
        area[par[i]]-=area[i];
    }
    for (int i=0; i < h1;i++){
        aux = h[i];
        squared_gray_avg[i]=1.0*area[i]*(aux*aux);
    }
    
    for (int i=h1-1; i > 0;i--){
        squared_gray_avg[par[i]] += squared_gray_avg[i];
    }
    
}

 
    void compute_eccentricity_aux_c(int h1, double *xx, int h2, double *yy, int h3, double *xy,
                                    int h4, int *par, int H, int W, int *node_index){
        int index;
        int x,y;
        for (x = 0; x < H; x++){
            for(y = 0; y < W;y++){
                index = node_index[x*W+y];
                xx[index] += x*x;                     
                yy[index] += y*y;                   
                xy[index] += x*y;        
                            
                } 
            }    
        
        for (int i=h4-1; i > 0;i--){
            index = par[i];
            xx[index] += xx[i];
            yy[index] += yy[i];
            xy[index] += xy[i];
         }        
                
    } 











