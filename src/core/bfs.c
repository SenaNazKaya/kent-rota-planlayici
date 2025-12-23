#include "bfs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

BFSYol* bfsEnKisaYol(Graf* graf, int baslangic_id, int hedef_id) {
    int bas_index = durakIDileIndex(graf, baslangic_id);
    int hedef_index = durakIDileIndex(graf, hedef_id);
    
    if (bas_index == -1 || hedef_index == -1) {
        printf("HATA: Baslangic veya hedef durak bulunamadi!\n");
        return NULL;
    }
    
    if (bas_index == hedef_index) {
        printf("HATA: Baslangic ve hedef ayni durak!\n");
        return NULL;
    }
    
    // BFS için gerekli diziler
    int ziyaret[GRAF_BOYUT] = {0};
    int ebeveyn[GRAF_BOYUT];
    for (int i = 0; i < GRAF_BOYUT; i++) {
        ebeveyn[i] = -1;
    }
    
    Queue* q = queueOlustur();
    ziyaret[bas_index] = 1;
    queueEkle(q, bas_index);
    
    // BFS döngüsü
    while (!queueBosMu(q)) {
        int current = queueCikar(q);
        
        // Hedefe ulaştık mı?
        if (current == hedef_index) {
            break;
        }
        
        // Komşuları kuyruğa ekle
        Hat* kenar = graf->kenarlar[current];
        while (kenar) {
            int komsu_index = durakIDileIndex(graf, kenar->hedef_id);
            if (komsu_index != -1 && !ziyaret[komsu_index]) {
                ziyaret[komsu_index] = 1;
                ebeveyn[komsu_index] = current;
                queueEkle(q, komsu_index);
            }
            kenar = kenar->sonraki;
        }
    }
    
    queueTemizle(q);
    
    // Yolu oluştur
    if (ebeveyn[hedef_index] == -1) {
        printf("HATA: Baslangic ile hedef arasinda yol yok!\n");
        return NULL;
    }
    
    // Yol uzunluğunu bul
    int yol_uzunluk = 0;
    int current = hedef_index;
    while (current != -1) {
        yol_uzunluk++;
        current = ebeveyn[current];
    }
    
    // Yol dizisini oluştur
    BFSYol* bfs_yol = (BFSYol*)malloc(sizeof(BFSYol));
    bfs_yol->yol = (int*)malloc(yol_uzunluk * sizeof(int));
    bfs_yol->uzunluk = yol_uzunluk;
    
    // Yolu tersten doldur
    current = hedef_index;
    for (int i = yol_uzunluk - 1; i >= 0; i--) {
        bfs_yol->yol[i] = graf->duraklar[current]->id;
        current = ebeveyn[current];
    }
    
    // Mesafe ve süreyi hesapla
    bfs_yol->mesafe = 0;
    bfs_yol->sure = 0;
    bfs_yol->aktarma = 0;
    
    char onceki_hat[20] = "";
    for (int i = 0; i < yol_uzunluk - 1; i++) {
        int from_id = bfs_yol->yol[i];
        int to_id = bfs_yol->yol[i + 1];
        int from_index = durakIDileIndex(graf, from_id);
        
        // Kenar bilgilerini bul
        Hat* kenar = graf->kenarlar[from_index];
        while (kenar) {
            if (kenar->hedef_id == to_id) {
                bfs_yol->mesafe += kenar->mesafe;
                bfs_yol->sure += kenar->sure;
                
                // Aktarma sayısını güncelle
                if (strcmp(onceki_hat, "") != 0 && 
                    strcmp(onceki_hat, kenar->hat_adi) != 0) {
                    bfs_yol->aktarma++;
                }
                strcpy(onceki_hat, kenar->hat_adi);
                break;
            }
            kenar = kenar->sonraki;
        }
    }
    
    return bfs_yol;
}

void bfsYolYazdir(BFSYol* yol, Graf* graf) {
    if (!yol) return;
    
    printf("\n=== BFS ILE EN AZ AKTARMALI YOL ===\n");
    printf("Toplam mesafe: %d km\n", yol->mesafe);
    printf("Toplam sure: %d dakika\n", yol->sure);
    printf("Aktarma sayisi: %d\n", yol->aktarma);
    printf("Toplam durak sayisi: %d\n", yol->uzunluk);
    printf("\nROTA: ");
    
    for (int i = 0; i < yol->uzunluk; i++) {
        int durak_id = yol->yol[i];
        int index = durakIDileIndex(graf, durak_id);
        if (index != -1) {
            printf("%s", graf->duraklar[index]->ad);
        } else {
            printf("ID:%d", durak_id);
        }
        
        if (i < yol->uzunluk - 1) {
            printf(" -> ");
        }
    }
    printf("\n");
}

void bfsYolTemizle(BFSYol* yol) {
    if (yol) {
        if (yol->yol) free(yol->yol);
        free(yol);
    }
}