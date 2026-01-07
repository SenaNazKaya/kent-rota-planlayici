// durak.c (GÜNCELLENMİŞ HALİ)
#include "durak.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ================= TRIE (opsiyonel) ================= */
TrieDugum* yeniTrieDugum() {
    TrieDugum* dugum = (TrieDugum*)malloc(sizeof(TrieDugum));
    for (int i = 0; i < 29; i++) dugum->harf[i] = NULL;
    dugum->durak_adi = false;
    dugum->durak_id = -1;
    return dugum;
}

/* ================= DURAK YAZDIR ================= */
void duraklariYazdir(Durak* bas) {
    printf("\n=== DURAKLAR ===\n");
    Durak* gecici = bas;
    int sayac = 0;
    while (gecici) {
        printf("%d. ID: %d, Ad: %s\n", ++sayac, gecici->id, gecici->ad);
        gecici = gecici->sonraki;
    }
    printf("Toplam %d durak\n", sayac);
}

/* ================= HAT YAZDIR ================= */
void hatlariYazdir(Hat* bas) {
    printf("\n=== HATLAR ===\n");
    Hat* gecici = bas;
    int sayac = 0;
    while (gecici) {
        printf("%d. %d -> %d: %d km, %d dak, Hat: %s\n", 
               ++sayac, gecici->baslangic_id, gecici->hedef_id,
               gecici->mesafe, gecici->sure, gecici->hat_adi);
        gecici = gecici->sonraki;
    }
    printf("Toplam %d hat\n", sayac);
}