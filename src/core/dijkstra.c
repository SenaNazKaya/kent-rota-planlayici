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

    int* mesafe = (int*)malloc(V * sizeof(int));
    int* sure = (int*)malloc(V * sizeof(int));
    int* ebeveyn = (int*)malloc(V * sizeof(int));
    int* ziyaret = (int*)malloc(V * sizeof(int));

    for (int i = 0; i < V; i++) {
        mesafe[i] = INT_MAX;
        sure[i] = INT_MAX;
        ebeveyn[i] = -1;
        ziyaret[i] = 0;
    }

    mesafe[bas_index] = 0;
    sure[bas_index] = 0;

    MinHeap* heap = minHeapOlustur(V);
    minHeapEkle(heap, bas_index, 0);

    // ================= DIJKSTRA =================
    while (!minHeapBosMu(heap)) {
        MinHeapNode* minNode = minHeapCikar(heap);
        int u = minNode->dugum_index;
        free(minNode);

        ziyaret[u] = 1;

        Hat* kenar = graf->kenarlar[u];
        while (kenar) {
            int v = durakIDileIndex(graf, kenar->hedef_id);

            if (v != -1 && !ziyaret[v]) {
                int yeni_mesafe = mesafe[u] + kenar->mesafe;
                int yeni_sure = sure[u] + kenar->sure;

                if (kriter == 0) { // MESAFE
                    if (yeni_mesafe < mesafe[v]) {
                        mesafe[v] = yeni_mesafe;
                        sure[v] = yeni_sure;
                        ebeveyn[v] = u;

                        if (minHeapVarmi(heap, v))
                            minHeapGuncelle(heap, v, yeni_mesafe);
                        else
                            minHeapEkle(heap, v, yeni_mesafe);
                    }
                } else { // SÜRE
                    if (yeni_sure < sure[v]) {
                        sure[v] = yeni_sure;
                        mesafe[v] = yeni_mesafe;
                        ebeveyn[v] = u;

                        if (minHeapVarmi(heap, v))
                            minHeapGuncelle(heap, v, yeni_sure);
                        else
                            minHeapEkle(heap, v, yeni_sure);
                    }
                }
            }
            kenar = kenar->sonraki;
        }
    }

    minHeapTemizle(heap);

    if (ebeveyn[hedef_index] == -1) {
        printf("HATA: Baslangic ile hedef arasinda yol yok!\n");
        free(mesafe);
        free(sure);
        free(ebeveyn);
        free(ziyaret);
        return NULL;
    }

    // ================= YOL OLUŞTUR =================
    int yol_uzunluk = 0;
    int current = hedef_index;
    while (current != -1) {
        yol_uzunluk++;
        current = ebeveyn[current];
    }

    DijkstraYol* dijkstra_yol = (DijkstraYol*)malloc(sizeof(DijkstraYol));
    dijkstra_yol->yol = (int*)malloc(yol_uzunluk * sizeof(int));
    dijkstra_yol->uzunluk = yol_uzunluk;
    dijkstra_yol->mesafe = mesafe[hedef_index];
    dijkstra_yol->sure = sure[hedef_index];

    current = hedef_index;
    for (int i = yol_uzunluk - 1; i >= 0; i--) {
        dijkstra_yol->yol[i] = graf->duraklar[current]->id;
        current = ebeveyn[current];
    }

    // ================= AKTARMA (HAT SAYISI) =================
    dijkstra_yol->aktarma = 0;
    char onceki_hat[20] = "";

    for (int i = 0; i < yol_uzunluk - 1; i++) {
        int from_id = dijkstra_yol->yol[i];
        int to_id   = dijkstra_yol->yol[i + 1];
        int from_index = durakIDileIndex(graf, from_id);

        Hat* kenar = graf->kenarlar[from_index];
        while (kenar) {
            if (kenar->hedef_id == to_id) {

                // İlk hat
                if (strcmp(onceki_hat, "") == 0) {
                    dijkstra_yol->aktarma = 1;
                    strcpy(onceki_hat, kenar->hat_adi);
                }
                // Hat değiştiyse
                else if (strcmp(onceki_hat, kenar->hat_adi) != 0) {
                    dijkstra_yol->aktarma++;
                    strcpy(onceki_hat, kenar->hat_adi);
                }
                break;
            }
            kenar = kenar->sonraki;
        }
    }

    free(mesafe);
    free(sure);
    free(ebeveyn);
    free(ziyaret);

    return dijkstra_yol;
}

/* ================= YAZDIR ================= */
void dijkstraYolYazdir(DijkstraYol* yol, Graf* graf) {
    if (!yol) return;

    printf("\n=== DIJKSTRA ===\n");
    printf("Mesafe: %d km\n", yol->mesafe);
    printf("Sure: %d dk\n", yol->sure);
    printf("Kullanilan hat sayisi: %d\n", yol->aktarma);

    printf("Rota: ");
    for (int i = 0; i < yol->uzunluk; i++) {
        int index = durakIDileIndex(graf, yol->yol[i]);
        if (index != -1)
            printf("%s", graf->duraklar[index]->ad);
        else
            printf("%d", yol->yol[i]);

        if (i < yol->uzunluk - 1) printf(" -> ");
    }
    printf("\n");
}

void dijkstraYolTemizle(DijkstraYol* yol) {
    if (yol) {
        if (yol->yol) free(yol->yol);
        free(yol);
    }
}
void rotaHatBazliYazdir(int* yol, int uzunluk, Graf* graf) {
    if (!yol || uzunluk < 2) return;

    char onceki_hat[50] = "";
    int bas_index;

    printf("\n=== HAT BAZLI ROTA ===\n");

    for (int i = 0; i < uzunluk - 1; i++) {
        int from_id = yol[i];
        int to_id   = yol[i + 1];

        bas_index = durakIDileIndex(graf, from_id);
        if (bas_index == -1) continue;

        Hat* kenar = graf->kenarlar[bas_index];
        while (kenar) {
            if (kenar->hedef_id == to_id) {

                // Hat değiştiyse yeni başlık
                if (strcmp(onceki_hat, kenar->hat_adi) != 0) {

                    // Önceki satırı kapat
                    if (strcmp(onceki_hat, "") != 0) {
                        printf("\n");
                    }

                    printf("[%s] ", kenar->hat_adi);
                    strcpy(onceki_hat, kenar->hat_adi);
                }

                // Durak adlarını yaz
                int from_idx = durakIDileIndex(graf, from_id);
                int to_idx   = durakIDileIndex(graf, to_id);

                if (from_idx != -1 && to_idx != -1) {
                    printf("%s → %s",
                           graf->duraklar[from_idx]->ad,
                           graf->duraklar[to_idx]->ad);
                }

                printf(" | ");
                break;
            }
            kenar = kenar->sonraki;
        }
    }
}
