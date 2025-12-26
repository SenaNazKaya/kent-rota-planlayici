#include "durak.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Graf fonksiyonları zaten graf.c'de
// Sadece gerekli fonksiyonları ekleyelim



// Trie için basit fonksiyon (opsiyonel)
TrieDugum* yeniTrieDugum() {
    TrieDugum* dugum = (TrieDugum*)malloc(sizeof(TrieDugum));
    for (int i = 0; i < 29; i++) dugum->harf[i] = NULL;
    dugum->durak_adi = false;
    dugum->durak_id = -1;
    return dugum;
}
