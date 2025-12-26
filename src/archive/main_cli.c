#include "durak.h"
#include "graf.h"
#include "bfs.h"
#include "dijkstra.h"
#include <stdio.h>
#include <stdlib.h>

extern Durak* duraklariOku(const char* dosya_adi);
extern Hat* hatlariOku(const char* dosya_adi);
extern void duraklariYazdir(Durak* bas);
extern void hatlariYazdir(Hat* bas);

void menuGoster() {
    printf("\n=== KENT İÇİ ROTA PLANLAYICI ===\n");
    printf("1. Duraklari goruntule\n");
    printf("2. Hatlari goruntule\n");
    printf("3. Graf yapisini goruntule\n");
    printf("4. BFS ile rota bul (en az aktarma)\n");
    printf("5. Dijkstra ile rota bul (en kisa mesafe)\n");
    printf("6. Dijkstra ile rota bul (en kisa sure)\n");
    printf("7. Cikis\n");
    printf("Seciminiz: ");
}

void rotaBulGoster(Graf* graf, int algoritma) {
    int baslangic_id, hedef_id;
    
    printf("\n--- ROTA BULMA ---\n");
    printf("Baslangic durak ID: ");
    scanf("%d", &baslangic_id);
    printf("Hedef durak ID: ");
    scanf("%d", &hedef_id);
    
    if (algoritma == 1) { // BFS
        BFSYol* yol = bfsEnKisaYol(graf, baslangic_id, hedef_id);
        if (yol) {
            bfsYolYazdir(yol, graf);
            bfsYolTemizle(yol);
        }
    } else if (algoritma == 2) { // Dijkstra mesafe
        DijkstraYol* yol = dijkstraEnKisaYol(graf, baslangic_id, hedef_id, 0);
        if (yol) {
            dijkstraYolYazdir(yol, graf);
            dijkstraYolTemizle(yol);
        }
    } else if (algoritma == 3) { // Dijkstra süre
        DijkstraYol* yol = dijkstraEnKisaYol(graf, baslangic_id, hedef_id, 1);
        if (yol) {
            dijkstraYolYazdir(yol, graf);
            dijkstraYolTemizle(yol);
        }
    }
}

int main() {
    printf("=== KENT İÇİ ROTA PLANLAYICI ===\n");
    printf("3. Hafta: Dijkstra Algoritmasi Eklendi\n\n");
    
    // 1. Dosyalardan verileri oku
    Durak* duraklar = duraklariOku("data/durak.csv");
    Hat* hatlar = hatlariOku("data/hat.csv");
    
    if (!duraklar || !hatlar) {
        printf("HATA: Veriler yuklenemedi!\n");
        return 1;
    }
    
    // 2. Graf oluştur
    Graf* graf = grafOlustur(duraklar, hatlar);
    if (!graf) {
        printf("HATA: Graf olusturulamadi!\n");
        return 1;
    }
    
    // 3. Ana menü döngüsü
    int secim;
    do {
        menuGoster();
        scanf("%d", &secim);
        
        switch(secim) {
            case 1:
                duraklariYazdir(duraklar);
                break;
                
            case 2:
                hatlariYazdir(hatlar);
                break;
                
            case 3:
                grafYazdir(graf);
                break;
                
            case 4:
                rotaBulGoster(graf, 1); // BFS
                break;
                
            case 5:
                rotaBulGoster(graf, 2); // Dijkstra mesafe
                break;
                
            case 6:
                rotaBulGoster(graf, 3); // Dijkstra süre
                break;
                
            case 7:
                printf("Program sonlandiriliyor...\n");
                break;
                
            default:
                printf("Gecersiz secim! Tekrar deneyin.\n");
        }
        
    } while (secim != 7);
    
    // 4. Belleği temizle
    grafTemizle(graf);
    
    // Durak ve hat listelerini temizle
    Durak* temp_durak;
    while (duraklar) {
        temp_durak = duraklar;
        duraklar = duraklar->sonraki;
        free(temp_durak);
    }
    
    Hat* temp_hat;
    while (hatlar) {
        temp_hat = hatlar;
        hatlar = hatlar->sonraki;
        free(temp_hat);
    }
    
    printf("\nProgram sonlandi.\n");
    return 0;
}