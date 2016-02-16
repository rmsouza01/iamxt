#include<iostream>
#include<vector>
#include<stdio.h>
#include<assert.h>
#include <unistd.h>
#include <deque>
#ifdef _OPENMP
   #include <omp.h>
#else
   #define omp_get_thread_num() 0
#endif




using namespace std;

// Returns the queue with priority = key
// if there is an element with this priority
// in the queue. If there is not it returns -1

int findSubQueue(vector<pair<deque<int>,int> > &HFQ, int key) {
    for (int i = 0; i < HFQ.size(); i++) {
        if (HFQ[i].second == key)
            return i;
    }
    return -1;
}
    
//Poe o elemento element na fila com prioridade key se
// ela já existir, senão a fila é criada

void Enqueue(vector<pair<deque<int>,int> > &HFQ, int element, int key) {
    int res = findSubQueue(HFQ, key);
    if (res == -1) {
        deque<int> inq;
        inq.push_back(element);
        HFQ.push_back(make_pair(inq, key));
    }
    else {
        HFQ[res].first.push_back(element);
    }
}

// Removes the element of minimum cost

int DequeueMin(vector<pair<deque<int>,int> > &HFQ){
    int key = 2147483647;
    int element = -1;
    int pos = -1;
    
    //Find the index of the lowest cost queue
    for (int i = 0; i < HFQ.size(); i++) {
        if (HFQ[i].second < key) {
            key = HFQ[i].second;
            pos = i;
        }
    }
    
    if (pos != -1) {            
        element =  HFQ[pos].first.front(); //first element of the queue in position pos
        HFQ[pos].first.pop_front(); // deletes the first element of the queue
        
        if (HFQ[pos].first.size() <= 0) //if the queue is now empty
            HFQ.erase(HFQ.begin()+pos); // erase it from vector
    }
    return element;
}

// Removes element p if element p
// exists in one of the queues

void Dequeue(vector<pair<deque<int>,int> > &HFQ,int p, int cp){
    int pos = findSubQueue(HFQ, cp);
    int element;
    if (pos != -1 && (!HFQ[pos].first.empty())){
        for (int i = 0; i < HFQ[pos].first.size(); i++){
            element = HFQ[pos].first[i];
            if (element == p){
                HFQ[pos].first.erase(HFQ[pos].first.begin()+i);
                if (HFQ[pos].first.size() <= 0){ //if the queue is now empty
                    HFQ.erase(HFQ.begin()+pos);
                }
                break;
            }
        }    
    }
}    

// Tie-zone watershed from markers
void ws_markers_c(int ii32,int H_off,int W_off, int *off,
                  int L,int M, int N, unsigned short *img,
                  int L_seg,int M_seg, int N_seg, int *seg,
                  int L_c1,int M_c1, int N_c1, int *C1,
                  int L_c2,int M_c2, int N_c2, int *C2,
                  int L_done, int M_done, int N_done, int *done,
                  int L_p, int M_p, int N_p, int *P){
        
        vector<pair<deque<int>,int> > HFQ; 
        int size = L*M*N;
        int MN = M*N;
        int h_p,v,p;
        int TZ = 0;
        int x,y,z,aux;
        int x_n,y_n,z_n;
        int n;
        unsigned short seg_p;
        int temp1,temp2,c;

        for (p = 0; p < size; p++){
            seg_p = seg[p];
            if (seg_p == 0){
                seg[p] = -1;
            }
            else {
                C1[p] = 0;
                P[p] = p;
                
                Enqueue(HFQ, p, 0); 
            }    
        } 
        while (!HFQ.empty()) {
            v = DequeueMin(HFQ);
                 
            done[v] = 1;
            
            x = v/MN;
            aux = (v-x*MN);
            y = aux/N;
            z = (aux)%N;
 
            for(int k = 0; k < W_off*H_off; k+=W_off){
                x_n = x + off[k];
                y_n = y + off[k + 1];
                z_n = z + off[k + 2];
                if ((x_n < 0) ||  (x_n >= L) ||  (y_n < 0) ||  (y_n >= M) ||
                   (z_n < 0) ||  (z_n >= N))
                    {continue;}   
                p = x_n*MN + y_n*N +z_n;
                if (done[p]!=0)
                    {continue;}
                temp1 = C1[v];
                temp2 = img[p];
                c = (temp1 > temp2) ? temp1 : temp2;
                if (c < C1[p]){
                    if (C1[p] < ii32){
                        Dequeue(HFQ,p,C1[p]);  
                           
                    }
                    C1[p] = c;
                    seg[p] = seg[v];
                    P[p] = v;
                    Enqueue(HFQ,p,C1[p]);
               }
           }
        } 
    HFQ.clear();
    } 

    // Tie-zone watershed from markers
    void tz_ws_c(int ii32,int H_off,int W_off, int *off,
                int L,int M, int N, unsigned short *img,
                int L_seg,int M_seg, int N_seg, int *seg,
                int L_c1,int M_c1, int N_c1, int *C1,
                int L_c2,int M_c2, int N_c2, int *C2,
                int L_done, int M_done, int N_done, int *done,
                int L_p, int M_p, int N_p, int *P){
        
        vector<pair<deque<int>,int> > HFQ; 
        int size = L*M*N;
        int MN = M*N;
        int h_p,v,p;
        int TZ = 0;
        int x,y,z,aux;
        int x_n,y_n,z_n;
        int n;
        unsigned short seg_p;
        int temp1,temp2,c;

        for (p = 0; p < size; p++){
            seg_p = seg[p];
            if (seg_p == 0){
                seg[p] = -1;
            }
            else {
               C1[p] = 0;
                P[p] = p;
                Enqueue(HFQ, p, 0); 
            }    
        } 
           
         while (!HFQ.empty()) {
            v = DequeueMin(HFQ);
                 
            done[v] = 1;
            
            x = v/MN;
            aux = (v-x*MN);
            y = aux/N;
            z = (aux)%N;
 
            for(int k = 0; k < W_off*H_off; k+=W_off){
                x_n = x + off[k];
                y_n = y + off[k + 1];
                z_n = z + off[k + 2];
                if ((x_n < 0) ||  (x_n >= L) ||  (y_n < 0) ||  (y_n >= M) ||
                   (z_n < 0) ||  (z_n >= N))
                    {continue;}   
                p = x_n*MN + y_n*N +z_n;
                if (done[p]!=0)
                    {continue;}
                temp1 = C1[v];
                temp2 = img[p];
                c = (temp1 > temp2) ? temp1 : temp2;
                if (c < C1[p]){
                    if (C1[p] < ii32){
                        Dequeue(HFQ,p,C1[p]);  
   
                    }
                    C1[p] = c;
                    seg[p] = seg[v];
                    P[p] = v;
                    Enqueue(HFQ,p,C1[p]);
                    if (c == C1[v]) C2[p] = C2[v] + 1;
                }
                else {
                    if (c ==C1[p] && seg[p]!=seg[v]){
                        if (c == C1[v])
                            {if (C2[p] == (C2[v]+1)) seg[p] = TZ;}
                        else 
                            {seg[p] = TZ;}
                    }
                }
            }
        } 
    HFQ.clear();
    } 


void window_histogram_aux_c(int L,int M, int N, int *f,
                            int H_off,int W_off, int *off,
                            int H1, int W1, int *pl,
                            int H2,int W2, int *hists){
        
        int MN = M*N;
        int x,y,z;
        int x_n,y_n,z_n;
        int q,p;
        int index1 = 0;
        int line = 0;
        
        for(int ii = 0; ii < H1; ii++){
            x = pl[index1++];
            y = pl[index1++];
            z = pl[index1++];
            q = x*MN + y*N +z;
            line = ii*W2;
            hists[line + f[q]]+=1;
            
            
            for(int k = 0; k < W_off*H_off; k+=W_off){
                x_n = x + off[k];
                y_n = y + off[k + 1];
                z_n = z + off[k + 2];
                if ((x_n < 0)||(x_n >= L)||(y_n < 0)||(y_n >= M)||(z_n < 0)||(z_n >= N)){
                    continue;
                    }   
                p = x_n*MN + y_n*N +z_n;
                hists[line + f[p]]+=1;
                }
            }
    } 
