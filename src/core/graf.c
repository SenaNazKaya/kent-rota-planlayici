#include "graf.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ID'ye göre düğüm indexini bul
int durakIDileIndex(Graf* graf, int durak_id) {
    for (int i = 0; i < graf->dugum_sayisi; i++) {
        if (graf->duraklar[i] && graf->duraklar[i]->id == durak_id) {
            return i;
        }
    }
    return -1;
}

// Graf oluştur
Graf* grafOlustur(Durak* duraklar, Hat* hatlar) {
    Graf* graf = (Graf*)malloc(sizeof(Graf));
    if (!graf) return NULL;
    
    graf->dugum_sayisi = 0;
    for (int i = 0; i < GRAF_BOYUT; i++) {
        graf->duraklar[i] = NULL;
        graf->kenarlar[i] = NULL;
        graf->komsu_sayisi[i] = 0;
    }
    
    // Durakları ekle
    Durak* durak_gez = duraklar;
    while (durak_gez && graf->dugum_sayisi < GRAF_BOYUT) {
        graf->duraklar[graf->dugum_sayisi] = durak_gez;
        graf->dugum_sayisi++;
        durak_gez = durak_gez->sonraki;
    }
    
    // Hatları ekle (çift yönlü)
    Hat* hat_gez = hatlar;
    while (hat_gez) {
        int bas_index = durakIDileIndex(graf, hat_gez->baslangic_id);
        int hedef_index = durakIDileIndex(graf, hat_gez->hedef_id);
        
        if (bas_index != -1 && hedef_index != -1) {
            // Gidiş yönü
            Hat* yeni_hat = (Hat*)malloc(sizeof(Hat));
            yeni_hat->baslangic_id = hat_gez->baslangic_id;
            yeni_hat->hedef_id = hat_gez->hedef_id;
            yeni_hat->mesafe = hat_gez->mesafe;
            yeni_hat->sure = hat_gez->sure;
            strcpy(yeni_hat->hat_adi, hat_gez->hat_adi);
            yeni_hat->sonraki = graf->kenarlar[bas_index];
            graf->kenarlar[bas_index] = yeni_hat;
            graf->komsu_sayisi[bas_index]++;
            
            // Geliş yönü
            Hat* ters_hat = (Hat*)malloc(sizeof(Hat));
            ters_hat->baslangic_id = hat_gez->hedef_id;
            ters_hat->hedef_id = hat_gez->baslangic_id;
            ters_hat->mesafe = hat_gez->mesafe;
            ters_hat->sure = hat_gez->sure;
            strcpy(ters_hat->hat_adi, hat_gez->hat_adi);
            ters_hat->sonraki = graf->kenarlar[hedef_index];
            graf->kenarlar[hedef_index] = ters_hat;
            graf->komsu_sayisi[hedef_index]++;
        }
        hat_gez = hat_gez->sonraki;
    }
    
    return graf;
}

// Grafı yazdır
void grafYazdir(Graf* graf) {
    printf("\n=== GRAF YAPISI ===\n");
    for (int i = 0; i < graf->dugum_sayisi; i++) {
        printf("%d. %s (ID: %d) -> ", 
               i+1, graf->duraklar[i]->ad, graf->duraklar[i]->id);
        
        Hat* kenar = graf->kenarlar[i];
        if (!kenar) {
            printf("Komsu yok\n");
            continue;
        }
        
        int sayac = 0;
        while (kenar) {
            int komsu_index = durakIDileIndex(graf, kenar->hedef_id);
            if (komsu_index != -1) {
                printf("%s", graf->duraklar[komsu_index]->ad);
            } else {
                printf("ID:%d", kenar->hedef_id);
            }
            
            printf("(%dkm) ", kenar->mesafe);
            kenar = kenar->sonraki;
            
            if (kenar) printf("-> ");
            sayac++;
            if (sayac > 5) { printf("..."); break; }
        }
        printf("\n");
    }
    printf("Toplam %d dugum\n", graf->dugum_sayisi);
}

// Grafı temizle
void grafTemizle(Graf* graf) {
    for (int i = 0; i < graf->dugum_sayisi; i++) {
        Hat* kenar = graf->kenarlar[i];
        while (kenar) {
            Hat* silinecek = kenar;
            kenar = kenar->sonraki;
            free(silinecek);
        }
    }
    free(graf);
}