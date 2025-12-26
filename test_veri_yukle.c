#include "durak.h"
#include "graf.h"
#include <stdio.h>

extern Durak* duraklariOku(const char* dosya_adi);
extern Hat* hatlariOku(const char* dosya_adi);
extern Graf* grafOlustur(Durak* duraklar, Hat* hatlar);

int main() {
    printf("=== VERI YUKLEME TESTI ===\n");
    
    printf("1. Duraklari yukle...\n");
    Durak* duraklar = duraklariOku("data/durak.csv");
    printf("   Duraklar: %p\n", duraklar);
    
    printf("2. Hatlari yukle...\n");
    Hat* hatlar = hatlariOku("data/hat.csv");
    printf("   Hatlar: %p\n", hatlar);
    
    if (!duraklar || !hatlar) {
        printf("❌ HATA: Veriler yuklenemedi!\n");
        return 1;
    }
    
    printf("3. Graf olustur...\n");
    Graf* graf = grafOlustur(duraklar, hatlar);
    printf("   Graf: %p\n", graf);
    
    if (graf) {
        printf("✅ BASARILI! Graf olusturuldu: %d dugum\n", graf->dugum_sayisi);
        
        // İlk 3 durağı göster
        printf("\nIlk 3 durak:\n");
        for (int i = 0; i < 3 && i < graf->dugum_sayisi; i++) {
            if (graf->duraklar[i]) {
                printf("  %d. %s (ID: %d)\n", 
                       i+1, graf->duraklar[i]->ad, graf->duraklar[i]->id);
            }
        }
    } else {
        printf("❌ HATA: Graf olusturulamadi!\n");
    }
    
    return 0;
}
