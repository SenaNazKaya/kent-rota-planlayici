#include "graf.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
Durak ID'leri:
id,ad
1,Taksim
2,Levent
3,Maslak
4,Kadikoy
5,Uskudar
6,Besiktas
7,Sisli
8,Atasehir
9,Yenikapi
10,Bostanci
*/

/* ================= ID -> INDEX ================= */

int durakIDileIndex(Graf* graf, int durak_id)
{
    int idx = durak_id - 1;
    if (idx < 0 || idx >= graf->dugum_sayisi) return -1;
    return idx;
}


/* ================= GRAF OLUSTUR ================= */

Graf* grafOlustur(Durak* duraklar, Hat* hatlar)
{
    Graf* graf = (Graf*)malloc(sizeof(Graf));
    if (!graf) return NULL;

    graf->dugum_sayisi = 0;

    for (int i = 0; i < GRAF_BOYUT; i++) {
        graf->duraklar[i] = NULL;
        graf->kenarlar[i] = NULL;
        graf->komsu_sayisi[i] = 0;
    }

    /* Duraklari ekle (ID-1 = INDEX) */
Durak* d = duraklar;
while (d && graf->dugum_sayisi < GRAF_BOYUT) {
    int idx = d->id - 1;
    graf->duraklar[idx] = d;
    graf->dugum_sayisi++;
    d = d->sonraki;
}

    /* Hatlari ekle (CIFT YONLU) */
    Hat* h = hatlar;
    while (h) {
        int b = durakIDileIndex(graf, h->baslangic_id);
        int t = durakIDileIndex(graf, h->hedef_id);

        if (b != -1 && t != -1) {

            /* Gidis */
            Hat* ileri = (Hat*)malloc(sizeof(Hat));
            *ileri = *h;
            ileri->sonraki = graf->kenarlar[b];
            graf->kenarlar[b] = ileri;
            graf->komsu_sayisi[b]++;

            /* Donus */
            Hat* geri = (Hat*)malloc(sizeof(Hat));
            *geri = *h;
            geri->baslangic_id = h->hedef_id;
            geri->hedef_id = h->baslangic_id;
            geri->sonraki = graf->kenarlar[t];
            graf->kenarlar[t] = geri;
            graf->komsu_sayisi[t]++;
        }

        h = h->sonraki;
    }

    return graf;
}

/* ================= GRAF YAZDIR ================= */

void grafYazdir(Graf* graf)
{
    printf("\n=== GRAF ===\n");

    for (int i = 0; i < graf->dugum_sayisi; i++) {
        printf("%s -> ", graf->duraklar[i]->ad);

        Hat* h = graf->kenarlar[i];
        while (h) {
            printf("%d(%s) ",
                   h->hedef_id,
                   h->hat_adi);
            h = h->sonraki;
            if (h) printf("-> ");
        }
        printf("\n");
    }
}

/* ================= GRAF TEMIZLE ================= */

void grafTemizle(Graf* graf)
{
    for (int i = 0; i < graf->dugum_sayisi; i++) {
        Hat* h = graf->kenarlar[i];
        while (h) {
            Hat* sil = h;
            h = h->sonraki;
            free(sil);
        }
    }
    free(graf);
}

/* ================= HAT BUL (GERCEKCI) ================= */
/*
ÖNCELİK SIRASI:
1️⃣ Marmaray (Boğaz geçişi = GERÇEK HAYAT)
2️⃣ Metro
3️⃣ Vapur
4️⃣ Diğer
*/

Hat* grafHatBul(Graf* graf, int from_id, int to_id)
{
    int idx = durakIDileIndex(graf, from_id);
    if (idx == -1) return NULL;

    Hat* h = graf->kenarlar[idx];

    Hat* marmaray = NULL;
    Hat* metro    = NULL;
    Hat* vapur    = NULL;
    Hat* diger    = NULL;

    while (h) {
        if (h->hedef_id == to_id) {

            if (strstr(h->hat_adi, "Marmaray"))
                marmaray = h;
            else if (strstr(h->hat_adi, "Metro"))
                metro = h;
            else if (strstr(h->hat_adi, "Vapur"))
                vapur = h;
            else
                diger = h;
        }
        h = h->sonraki;
    }

    if (marmaray) return marmaray;
    if (metro)    return metro;
    if (vapur)    return vapur;
    return diger;
}
/* ================= HAT BUL (BASIT) ================= */
Hat* grafHatBulBasit(Graf* graf, int from_id, int to_id) {
    int idx = durakIDileIndex(graf, from_id);
    if (idx == -1) return NULL;
    
    Hat* h = graf->kenarlar[idx];
    while (h) {
        if (h->hedef_id == to_id) {
            return h;
        }
        h = h->sonraki;
    }
    return NULL;
}