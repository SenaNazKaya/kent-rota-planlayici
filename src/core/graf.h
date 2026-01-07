#ifndef GRAF_H
#define GRAF_H

#include "durak.h"

// Fonksiyon bildirimleri
int durakIDileIndex(Graf* graf, int durak_id);
Graf* grafOlustur(Durak* duraklar, Hat* hatlar);
void grafYazdir(Graf* graf);
void grafTemizle(Graf* graf);
Hat* grafHatBul(Graf* graf, int from_id, int to_id);
Hat* grafHatBulBasit(Graf* graf, int from_id, int to_id);

#endif