#ifndef BFS_H
#define BFS_H

#include "graf.h"

typedef struct BFSYol {
    int* yol;           // Durak ID'leri
    int uzunluk;        // Durak sayısı
    int aktarma;        // Aktarma sayısı
    int mesafe;         // Toplam mesafe
    int sure;           // Toplam süre
} BFSYol;

BFSYol* bfsEnKisaYol(Graf* graf, int baslangic_id, int hedef_id);
void bfsYolTemizle(BFSYol* yol);

#endif
