#include "dijkstra.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

DijkstraYol* dijkstraEnKisaYol(Graf* graf, int baslangic_id, int hedef_id, int kriter) {
    // kriter: 0 = mesafe, 1 = süre
    
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
    
    int V = graf->dugum_sayisi;
    
    // Mesafe dizileri
    int* mesafe = (int*)malloc(V * sizeof(int));
    int* sure = (int*)malloc(V * sizeof(int));
    int* ebeveyn = (int*)malloc(V * sizeof(int));
    int* ziyaret = (int*)malloc(V * sizeof(int));
    
    // Başlangıç değerleri
    for (int i = 0; i < V; i++) {
        mesafe[i] = INT_MAX;
        sure[i] = INT_MAX;
        ebeveyn[i] = -1;
        ziyaret[i] = 0;
    }
    
    mesafe[bas_index] = 0;
    sure[bas_index] = 0;
    
    // MinHeap oluştur
    MinHeap* heap = minHeapOlustur(V);
    minHeapEkle(heap, bas_index, (kriter == 0) ? 0 : 0);
    
    // Dijkstra ana döngüsü
    while (!minHeapBosMu(heap)) {
        MinHeapNode* minNode = minHeapCikar(heap);
        int u = minNode->dugum_index;
        free(minNode);
        
        ziyaret[u] = 1;
        
        // Tüm komşuları kontrol et
        Hat* kenar = graf->kenarlar[u];
        while (kenar) {
            int v = durakIDileIndex(graf, kenar->hedef_id);
            
            if (v != -1 && !ziyaret[v]) {
                int yeni_mesafe = mesafe[u] + kenar->mesafe;
                int yeni_sure = sure[u] + kenar->sure;
                
                // Kriter seçimi
                if (kriter == 0) { // Mesafe
                    if (yeni_mesafe < mesafe[v]) {
                        mesafe[v] = yeni_mesafe;
                        sure[v] = yeni_sure;
                        ebeveyn[v] = u;
                        
                        if (minHeapVarmi(heap, v)) {
                            minHeapGuncelle(heap, v, yeni_mesafe);
                        } else {
                            minHeapEkle(heap, v, yeni_mesafe);
                        }
                    }
                } else { // Süre
                    if (yeni_sure < sure[v]) {
                        sure[v] = yeni_sure;
                        mesafe[v] = yeni_mesafe;
                        ebeveyn[v] = u;
                        
                        if (minHeapVarmi(heap, v)) {
                            minHeapGuncelle(heap, v, yeni_sure);
                        } else {
                            minHeapEkle(heap, v, yeni_sure);
                        }
                    }
                }
            }
            kenar = kenar->sonraki;
        }
    }
    
    minHeapTemizle(heap);
    
    // Yolu oluştur
    if (ebeveyn[hedef_index] == -1) {
        printf("HATA: Baslangic ile hedef arasinda yol yok!\n");
        free(mesafe);
        free(sure);
        free(ebeveyn);
        free(ziyaret);
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
    DijkstraYol* dijkstra_yol = (DijkstraYol*)malloc(sizeof(DijkstraYol));
    dijkstra_yol->yol = (int*)malloc(yol_uzunluk * sizeof(int));
    dijkstra_yol->uzunluk = yol_uzunluk;
    dijkstra_yol->mesafe = mesafe[hedef_index];
    dijkstra_yol->sure = sure[hedef_index];
    
    // Yolu tersten doldur
    current = hedef_index;
    for (int i = yol_uzunluk - 1; i >= 0; i--) {
        dijkstra_yol->yol[i] = graf->duraklar[current]->id;
        current = ebeveyn[current];
    }
    
    // Aktarma sayısını hesapla
    dijkstra_yol->aktarma = 0;
    char onceki_hat[20] = "";
    
    for (int i = 0; i < yol_uzunluk - 1; i++) {
        int from_id = dijkstra_yol->yol[i];
        int to_id = dijkstra_yol->yol[i + 1];
        int from_index = durakIDileIndex(graf, from_id);
        
        Hat* kenar = graf->kenarlar[from_index];
        while (kenar) {
            if (kenar->hedef_id == to_id) {
                if (strcmp(onceki_hat, "") != 0 && 
                    strcmp(onceki_hat, kenar->hat_adi) != 0) {
                    dijkstra_yol->aktarma++;
                }
                strcpy(onceki_hat, kenar->hat_adi);
                break;
            }
            kenar = kenar->sonraki;
        }
    }
    
    // Belleği temizle
    free(mesafe);
    free(sure);
    free(ebeveyn);
    free(ziyaret);
    
    return dijkstra_yol;
}

void dijkstraYolYazdir(DijkstraYol* yol, Graf* graf) {
    if (!yol) return;
    
    printf("\n=== DIJKSTRA ILE EN KISA YOL ===\n");
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

void dijkstraYolTemizle(DijkstraYol* yol) {
    if (yol) {
        if (yol->yol) free(yol->yol);
        free(yol);
    }
}