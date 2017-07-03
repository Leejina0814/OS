#include <cstdio>
#include <vector>
#include <string.h>
#include <cstring>
#include <algorithm>
using namespace std;

int num;
FILE *in;
FILE *ph;

int tlb_hit = 0;
int page_falut = 0;
int tlb_size = 0;
int frame_size = 0;
int total = 0;
int page_table[256];
int TLB[16];

vector <int> p_table;
vector <int> tlb;
vector <int>::iterator iter;


void insertPT(int p_n) {
 if (page_table[p_n] == -1) {
  page_falut++;
  if (frame_size < 128) {
   page_table[p_n] = frame_size;
   frame_size++;
   p_table.push_back(p_n);
  }
  else {
   page_table[p_n] = page_table[p_table.front()];
   page_table[p_table.front()] = -1;
   p_table.erase(p_table.begin());
   p_table.push_back(p_n);
  }
 }
 else {
  iter = find(p_table.begin(), p_table.end(), p_n);
  p_table.erase(iter);
  p_table.push_back(p_n);
 }
}

void search_tlb(int p_n) {
 iter = find(tlb.begin(), tlb.end(), p_n);
 if (iter == tlb.end()) {
  if (tlb_size < 16) {
   TLB[tlb_size] = p_n;
   tlb.push_back(p_n);
   tlb_size++;
  }
  else {
   
   int tlb_index;
   for (int i = 0; i < 16; i++) {
    if (TLB[i] == tlb.front()) {
     tlb_index = i;
     break;
    }
   }
   tlb.erase(tlb.begin());
   tlb.push_back(p_n);
   TLB[tlb_index] = p_n;
  }
  
 }
 else {
  tlb.erase(iter);
  tlb.push_back(p_n);
  tlb_hit++;
 }
 insertPT(p_n);
}

int main() {
 in = fopen("addresses.txt", "rb");
 memset(page_table, -1, sizeof(page_table));
 ph = fopen("Physical.txt", "wb");
 while (!feof(in)) {
  fscanf(in, "%d\n", &num);
  total++;
  int offset = num % 256;
  int p_num = num / 256;
  search_tlb(p_num);
  fprintf(ph, "%d \n", (page_table[p_num] * 256) + offset);
 
 }
 fprintf(ph,"TLB hit ratio : total ( %d ) , hit ( %d ) \n", total, tlb_hit);
 fprintf(ph,"Page fault ratio : total ( %d ), falut ( %d ) \n", total, page_falut);

 fclose(in);
 fclose(ph);
}

