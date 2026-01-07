#include "dijkstra.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

/* ================= HAT BONUS / CEZA ================= */
static int sureKatsayiGetir(const char* hat_adi) {
    if (strncmp(hat_adi, "Marmaray", 8) == 0)
        return -2;          // 🔥 tercih sebebi
    if (strncmp(hat_adi, "Metro", 5) == 0)
        return -1;          // hızlı
    if (strncmp(hat_adi, "Otobus", 6) == 0)
        return 3;           // trafik cezası
    return 0;               // Vapur vs
}
/* ==================================================== */

DijkstraYol* dijkstraEnKisaYol(Graf* graf, int baslangic_id, int hedef_id, int kriter) {
    // kriter: 0 = mesafe, 1 = süre

    int bas_index = durakIDileIndex(graf, baslangic_id);
    int hedef_index = durakIDileIndex(graf, hedef_id);

    if (bas_index == -1 || hedef_index == -1) return NULL;
    if (bas_index == hedef_index) return NULL;

    int V = graf->dugum_sayisi;

    int* mesafe = malloc(V * sizeof(int));
    int* sure   = malloc(V * sizeof(int));
    int* ebeveyn = malloc(V * sizeof(int));
    int* ziyaret = calloc(V, sizeof(int));

    for (int i = 0; i < V; i++) {
        mesafe[i] = INT_MAX;
        sure[i] = INT_MAX;
        ebeveyn[i] = -1;
    }

    mesafe[bas_index] = 0;
    sure[bas_index] = 0;

    MinHeap* heap = minHeapOlustur(V);
    minHeapEkle(heap, bas_index, 0);

    /* ================= DIJKSTRA ================= */
    while (!minHeapBosMu(heap)) {
        MinHeapNode* minNode = minHeapCikar(heap);
        int u = minNode->dugum_index;
        free(minNode);

        if (ziyaret[u]) continue;
        ziyaret[u] = 1;

        Hat* kenar = graf->kenarlar[u];
        while (kenar) {
            int v = durakIDileIndex(graf, kenar->hedef_id);

            if (v != -1 && !ziyaret[v]) {

                int bonus = 0;
                if (kriter == 1)  // sadece süre kriterinde
                    bonus = sureKatsayiGetir(kenar->hat_adi);

                int yeni_mesafe = mesafe[u] + kenar->mesafe;
                int yeni_sure   = sure[u] + kenar->sure + bonus;
                if (yeni_sure < 0) yeni_sure = 0; // negatif olmasın

                if (kriter == 0) { // MESAFE
                    if (yeni_mesafe < mesafe[v]) {
                        mesafe[v] = yeni_mesafe;
                        sure[v]   = yeni_sure;
                        ebeveyn[v] = u;

                        if (minHeapVarmi(heap, v))
                            minHeapGuncelle(heap, v, yeni_mesafe);
                        else
                            minHeapEkle(heap, v, yeni_mesafe);
                    }
                } else { // SÜRE
                    if (yeni_sure < sure[v]) {
                        sure[v]   = yeni_sure;
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
        free(mesafe); free(sure); free(ebeveyn); free(ziyaret);
        return NULL;
    }

    /* ================= YOL ================= */
    int len = 0;
    for (int v = hedef_index; v != -1; v = ebeveyn[v]) len++;

    DijkstraYol* yol = malloc(sizeof(DijkstraYol));
    yol->yol = malloc(len * sizeof(int));
    yol->uzunluk = len;
    yol->mesafe = mesafe[hedef_index];
    yol->sure   = sure[hedef_index];

    int i = len - 1;
    for (int v = hedef_index; v != -1; v = ebeveyn[v])
        yol->yol[i--] = graf->duraklar[v]->id;

    /* ================= AKTARMA ================= */
    yol->aktarma = 0;
    char onceki_hat[32] = "";

    for (int i = 0; i < len - 1; i++) {
        int u = durakIDileIndex(graf, yol->yol[i]);
        int v_id = yol->yol[i + 1];

        Hat* h = graf->kenarlar[u];
        while (h) {
            if (h->hedef_id == v_id) {
                if (onceki_hat[0] != '\0' &&
                    strcmp(onceki_hat, h->hat_adi) != 0)
                    yol->aktarma++;

                strcpy(onceki_hat, h->hat_adi);
                break;
            }
            h = h->sonraki;
        }
    }

    free(mesafe); free(sure); free(ebeveyn); free(ziyaret);
    return yol;
}

void dijkstraYolTemizle(DijkstraYol* yol)
{
    if (!yol) return;

    if (yol->yol)
        free(yol->yol);

    free(yol);
}

