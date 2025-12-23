#ifndef BFS_H
#define BFS_H

#include "graf.h"
#include "queue.h"

// BFS ile en kısa yolu bul (en az aktarma)
typedef struct BFSYol {
    int* yol;           // Durak ID'leri
    int uzunluk;        // Yol uzunluğu (durak sayısı)
    int aktarma;        // Aktarma sayısı (farklı hat sayısı)
    int mesafe;         // Toplam mesafe
    int sure;           // Toplam süre
} BFSYol;

// BFS fonksiyonları
BFSYol* bfsEnKisaYol(Graf* graf, int baslangic_id, int hedef_id);
void bfsYolYazdir(BFSYol* yol, Graf* graf);
void bfsYolTemizle(BFSYol* yol);

#endif