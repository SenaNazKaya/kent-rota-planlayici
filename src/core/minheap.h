#ifndef MINHEAP_H
#define MINHEAP_H

typedef struct MinHeapNode {
    int dugum_index;
    int mesafe;
} MinHeapNode;

typedef struct MinHeap {
    int size;
    int capacity;
    int* pos; // Düğümlerin heap'teki pozisyonu
    MinHeapNode** array;
} MinHeap;

// MinHeap fonksiyonları
MinHeap* minHeapOlustur(int capacity);
void minHeapEkle(MinHeap* heap, int dugum_index, int mesafe);
MinHeapNode* minHeapCikar(MinHeap* heap);
void minHeapGuncelle(MinHeap* heap, int dugum_index, int mesafe);
int minHeapBosMu(MinHeap* heap);
void minHeapTemizle(MinHeap* heap);
int minHeapVarmi(MinHeap* heap, int dugum_index);

#endif