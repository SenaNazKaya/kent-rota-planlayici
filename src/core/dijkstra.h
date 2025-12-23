#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "graf.h"
#include "minheap.h"

// Dijkstra yolu
typedef struct DijkstraYol {
    int* yol;           // Durak ID'leri
    int uzunluk;        // Yol uzunluğu
    int mesafe;         // Toplam mesafe
    int sure;           // Toplam süre
    int aktarma;        // Aktarma sayısı
} DijkstraYol;

// Dijkstra fonksiyonları
DijkstraYol* dijkstraEnKisaYol(Graf* graf, int baslangic_id, int hedef_id, int kriter);
void dijkstraYolYazdir(DijkstraYol* yol, Graf* graf);
void dijkstraYolTemizle(DijkstraYol* yol);

#endif