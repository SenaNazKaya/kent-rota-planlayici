#include "minheap.h"
#include <stdlib.h>
#include <stdio.h>

// Yeni heap düğümü
MinHeapNode* yeniMinHeapNode(int dugum_index, int mesafe) {
    MinHeapNode* node = (MinHeapNode*)malloc(sizeof(MinHeapNode));
    node->dugum_index = dugum_index;
    node->mesafe = mesafe;
    return node;
}

// Heap oluştur
MinHeap* minHeapOlustur(int capacity) {
    MinHeap* heap = (MinHeap*)malloc(sizeof(MinHeap));
    heap->pos = (int*)malloc(capacity * sizeof(int));
    heap->size = 0;
    heap->capacity = capacity;
    heap->array = (MinHeapNode**)malloc(capacity * sizeof(MinHeapNode*));
    
    for (int i = 0; i < capacity; i++) {
        heap->pos[i] = -1; // Başlangıçta yok
    }
    
    return heap;
}

// İki düğümü değiştir
void takasMinHeapNode(MinHeapNode** a, MinHeapNode** b) {
    MinHeapNode* t = *a;
    *a = *b;
    *b = t;
}

// Heapify işlemi
void minHeapify(MinHeap* heap, int idx) {
    int en_kucuk = idx;
    int sol = 2 * idx + 1;
    int sag = 2 * idx + 2;
    
    if (sol < heap->size && 
        heap->array[sol]->mesafe < heap->array[en_kucuk]->mesafe) {
        en_kucuk = sol;
    }
    
    if (sag < heap->size && 
        heap->array[sag]->mesafe < heap->array[en_kucuk]->mesafe) {
        en_kucuk = sag;
    }
    
    if (en_kucuk != idx) {
        // Pozisyonları güncelle
        heap->pos[heap->array[en_kucuk]->dugum_index] = idx;
        heap->pos[heap->array[idx]->dugum_index] = en_kucuk;
        
        // Düğümleri değiştir
        takasMinHeapNode(&heap->array[en_kucuk], &heap->array[idx]);
        
        // Rekürsif olarak devam et
        minHeapify(heap, en_kucuk);
    }
}

// Heap'ten minimum değeri çıkar
MinHeapNode* minHeapCikar(MinHeap* heap) {
    if (minHeapBosMu(heap)) {
        return NULL;
    }
    
    MinHeapNode* kok = heap->array[0];
    MinHeapNode* sonNode = heap->array[heap->size - 1];
    
    heap->array[0] = sonNode;
    
    // Pozisyonları güncelle
    heap->pos[kok->dugum_index] = heap->size - 1;
    heap->pos[sonNode->dugum_index] = 0;
    
    heap->size--;
    minHeapify(heap, 0);
    
    return kok;
}

// Mesafeyi güncelle
void minHeapGuncelle(MinHeap* heap, int dugum_index, int mesafe) {
    int i = heap->pos[dugum_index];
    heap->array[i]->mesafe = mesafe;
    
    // Yukarı doğru düzelt
    while (i && heap->array[i]->mesafe < heap->array[(i - 1) / 2]->mesafe) {
        heap->pos[heap->array[i]->dugum_index] = (i - 1) / 2;
        heap->pos[heap->array[(i - 1) / 2]->dugum_index] = i;
        takasMinHeapNode(&heap->array[i], &heap->array[(i - 1) / 2]);
        
        i = (i - 1) / 2;
    }
}

// Heap'e ekle
void minHeapEkle(MinHeap* heap, int dugum_index, int mesafe) {
    if (heap->pos[dugum_index] != -1) {
        // Zaten varsa güncelle
        minHeapGuncelle(heap, dugum_index, mesafe);
        return;
    }
    
    heap->size++;
    int i = heap->size - 1;
    heap->array[i] = yeniMinHeapNode(dugum_index, mesafe);
    heap->pos[dugum_index] = i;
    
    // Yukarı doğru düzelt
    while (i && heap->array[i]->mesafe < heap->array[(i - 1) / 2]->mesafe) {
        heap->pos[heap->array[i]->dugum_index] = (i - 1) / 2;
        heap->pos[heap->array[(i - 1) / 2]->dugum_index] = i;
        takasMinHeapNode(&heap->array[i], &heap->array[(i - 1) / 2]);
        
        i = (i - 1) / 2;
    }
}

// Heap boş mu?
int minHeapBosMu(MinHeap* heap) {
    return heap->size == 0;
}

// Heap'te var mı?
int minHeapVarmi(MinHeap* heap, int dugum_index) {
    return heap->pos[dugum_index] != -1;
}

// Heap'i temizle
void minHeapTemizle(MinHeap* heap) {
    for (int i = 0; i < heap->size; i++) {
        free(heap->array[i]);
    }
    free(heap->array);
    free(heap->pos);
    free(heap);
}