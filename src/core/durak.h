#ifndef DURAK_H
#define DURAK_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

// ncurses.h'yi KALDIRIYORUZ çünkü raylib ile çakışıyor
// #include <ncurses.h>

#define MAX_NAME 50
#define MAX_HAT_ADI 20
#define MAX_LINE 200
#define GRAF_BOYUT 100

typedef struct Durak {
    int id;
    char ad[MAX_NAME];
    struct Durak* sonraki;
} Durak;

typedef struct Hat {
    int baslangic_id;
    int hedef_id;
    int mesafe;
    int sure;
    char hat_adi[MAX_HAT_ADI];
    struct Hat* sonraki;
} Hat;

typedef struct Graf {
    int dugum_sayisi;
    Durak* duraklar[GRAF_BOYUT];
    Hat* kenarlar[GRAF_BOYUT];
    int komsu_sayisi[GRAF_BOYUT];
} Graf;

typedef struct TrieDugum {
    struct TrieDugum* harf[29];
    bool durak_adi;
    int durak_id;
} TrieDugum;

typedef struct Rota {
    int* yol;
    int yol_uzunluk;
    int mesafe;
    int sure;
    int aktarma;
    char hat_listesi[200];
} Rota;

typedef struct StackNode {
    Rota* rota;
    struct StackNode* next;
} StackNode;

typedef struct Stack {
    StackNode* top;
    int count;
} Stack;

// Graf fonksiyonları
Graf* grafOlustur(Durak* duraklar, Hat* hatlar);
void grafYazdir(Graf* graf);
void grafTemizle(Graf* graf);
int durakIDileIndex(Graf* graf, int durak_id);

// CSV okuma fonksiyonları - BUNLARI KALDIRIYORUZ çünkü fileio.c'de
// Durak* duraklariOku(const char* dosya_adi);
// Hat* hatlariOku(const char* dosya_adi);

// Yeni: Yazdırma fonksiyonları (durak.c'de olacak)
void duraklariYazdir(Durak* bas);
void hatlariYazdir(Hat* bas);

#endif