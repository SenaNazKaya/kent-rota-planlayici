#include "durak.h"

Durak* duraklariOku(const char* dosya_adi) {
    FILE* file = fopen(dosya_adi, "r");
    if (!file) {
        printf("Dosya acilamadi: %s\n", dosya_adi);
        return NULL;
    }
    
    char line[MAX_LINE];
    Durak* bas = NULL;
    Durak* son = NULL;
    
    fgets(line, sizeof(line), file);
    
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0;
        
        char* token = strtok(line, ",");
        if (!token) continue;
        int id = atoi(token);
        
        token = strtok(NULL, ",");
        if (!token) continue;
        
        Durak* yeni_durak = (Durak*)malloc(sizeof(Durak));
        yeni_durak->id = id;
        strncpy(yeni_durak->ad, token, MAX_NAME - 1);
        yeni_durak->ad[MAX_NAME - 1] = '\0';
        yeni_durak->sonraki = NULL;
        
        if (!bas) {
            bas = yeni_durak;
            son = yeni_durak;
        } else {
            son->sonraki = yeni_durak;
            son = yeni_durak;
        }
    }
    
    fclose(file);
    printf("%s dosyasindan duraklar yuklendi.\n", dosya_adi);
    return bas;
}

Hat* hatlariOku(const char* dosya_adi) {
    FILE* file = fopen(dosya_adi, "r");
    if (!file) {
        printf("Dosya acilamadi: %s\n", dosya_adi);
        return NULL;
    }
    
    char line[MAX_LINE];
    Hat* bas = NULL;
    Hat* son = NULL;
    
    fgets(line, sizeof(line), file);
    
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0;
        
        char* token = strtok(line, ",");
        if (!token) continue;
        int baslangic = atoi(token);
        
        token = strtok(NULL, ",");
        if (!token) continue;
        int hedef = atoi(token);
        
        token = strtok(NULL, ",");
        if (!token) continue;
        int mesafe = atoi(token);
        
        token = strtok(NULL, ",");
        if (!token) continue;
        int sure = atoi(token);
        
        token = strtok(NULL, ",");
        if (!token) continue;
        
        Hat* yeni_hat = (Hat*)malloc(sizeof(Hat));
        yeni_hat->baslangic_id = baslangic;
        yeni_hat->hedef_id = hedef;
        yeni_hat->mesafe = mesafe;
        yeni_hat->sure = sure;
        strncpy(yeni_hat->hat_adi, token, MAX_HAT_ADI - 1);
        yeni_hat->hat_adi[MAX_HAT_ADI - 1] = '\0';
        yeni_hat->sonraki = NULL;
        
        if (!bas) {
            bas = yeni_hat;
            son = yeni_hat;
        } else {
            son->sonraki = yeni_hat;
            son = yeni_hat;
        }
    }
    
    fclose(file);
    printf("%s dosyasindan hatlar yuklendi.\n", dosya_adi);
    return bas;
}

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