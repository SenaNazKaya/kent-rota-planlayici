#include "bfs.h"
#include "queue.h"
#include <stdlib.h>
#include <string.h>

/* ================== YARDIMCI ================== */

static void tasimaTuruGetir(const char* hat_adi, char* tur) {
    if (strncmp(hat_adi, "Metro", 5) == 0)
        strcpy(tur, "Metro");
    else if (strncmp(hat_adi, "Marmaray", 8) == 0)
        strcpy(tur, "Marmaray");
    else if (strncmp(hat_adi, "Otobus", 6) == 0)
        strcpy(tur, "Otobus");
    else if (strncmp(hat_adi, "Vapur", 5) == 0)
        strcpy(tur, "Vapur");
    else
        strcpy(tur, "Diger");
}

static void hatKoduGetir(const char* hat_adi, char* kod) {
    const char* tire = strchr(hat_adi, '-');
    if (tire)
        strcpy(kod, tire + 1);
    else
        strcpy(kod, hat_adi);
}

/* ============================================== */

BFSYol* bfsEnKisaYol(Graf* graf, int baslangic_id, int hedef_id) {

    int bas = durakIDileIndex(graf, baslangic_id);
    int hedef = durakIDileIndex(graf, hedef_id);
    if (bas == -1 || hedef == -1) return NULL;

    int V = graf->dugum_sayisi;

    int* visited = calloc(V, sizeof(int));
    int* parent  = malloc(V * sizeof(int));
    int* mesafe  = calloc(V, sizeof(int));
    int* sure    = calloc(V, sizeof(int));

    for (int i = 0; i < V; i++) parent[i] = -1;

    Queue* q = queueOlustur();
    visited[bas] = 1;
    queueEkle(q, bas);

    /* ================= BFS ================= */
    while (!queueBosMu(q)) {
        int u = queueCikar(q);
        if (u == hedef) break;

        Hat* h = graf->kenarlar[u];
        while (h) {
            int v = durakIDileIndex(graf, h->hedef_id);
            if (v != -1 && !visited[v]) {
                visited[v] = 1;
                parent[v]  = u;
                mesafe[v]  = mesafe[u] + h->mesafe;
                sure[v]    = sure[u] + h->sure;
                queueEkle(q, v);
            }
            h = h->sonraki;
        }
    }

    if (parent[hedef] == -1) {
        queueTemizle(q);
        free(visited); free(parent); free(mesafe); free(sure);
        return NULL;
    }

    /* ================= YOL ================= */
    int len = 0;
    for (int v = hedef; v != -1; v = parent[v]) len++;

    BFSYol* yol = malloc(sizeof(BFSYol));
    yol->yol = malloc(len * sizeof(int));
    yol->uzunluk = len;
    yol->mesafe = mesafe[hedef];
    yol->sure   = sure[hedef];

    int i = len - 1;
    for (int v = hedef; v != -1; v = parent[v])
        yol->yol[i--] = graf->duraklar[v]->id;

    /* ================= AKTARMA (GERÇEKÇİ) ================= */
    int aktarma = 0;
    char onceki_tur[20] = "";
    char onceki_kod[20] = "";

    for (int i = 0; i < len - 1; i++) {
        int u = durakIDileIndex(graf, yol->yol[i]);
        int v_id = yol->yol[i + 1];

        Hat* h = graf->kenarlar[u];
        while (h) {
            if (h->hedef_id == v_id) {

                char tur[20], kod[20];
                tasimaTuruGetir(h->hat_adi, tur);
                hatKoduGetir(h->hat_adi, kod);

                if (onceki_tur[0] != '\0') {
                    if (strcmp(onceki_tur, tur) != 0 ||
                        strcmp(onceki_kod, kod) != 0) {
                        aktarma++;
                    }
                }

                strcpy(onceki_tur, tur);
                strcpy(onceki_kod, kod);
                break;
            }
            h = h->sonraki;
        }
    }

    yol->aktarma = aktarma;

    queueTemizle(q);
    free(visited); free(parent); free(mesafe); free(sure);

    return yol;
}

void bfsYolTemizle(BFSYol* yol) {
    if (!yol) return;
    free(yol->yol);
    free(yol);
}

