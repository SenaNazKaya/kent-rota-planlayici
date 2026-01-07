#include "core/durak.h"
#include "core/graf.h"
#include "core/bfs.h"
#include "core/dijkstra.h"
#include "gui/gui.h"

#include <stdio.h>
#include <unistd.h>  // access() fonksiyonu için

extern Durak* duraklariOku(const char* dosya_adi);
extern Hat* hatlariOku(const char* dosya_adi);
extern Graf* grafOlustur(Durak* duraklar, Hat* hatlar);
extern void grafTemizle(Graf* graf);

int main() {
    printf("=== ROTA PLANLAYICI BAŞLATILIYOR ===\n");
    
    // 1. DOSYA KONTROLÜ
    printf("1. CSV dosyalari kontrol ediliyor...\n");
    if (access("data/durak.csv", F_OK) == -1) {
        printf("HATA: data/durak.csv bulunamadi!\n");
        printf("Lütfen data/ klasörü oluşturun ve CSV dosyalarını içine koyun.\n");
        return 1;
    }
    if (access("data/hat.csv", F_OK) == -1) {
        printf("HATA: data/hat.csv bulunamadi!\n");
        return 1;
    }
    printf("  ✓ CSV dosyalari bulundu\n");
    
    // 2. VERİLERİ YÜKLE
    printf("2. Duraklar yukleniyor...\n");
    Durak* duraklar = duraklariOku("data/durak.csv");
    if (!duraklar) {
        printf("HATA: Duraklar yuklenemedi!\n");
        return 1;
    }
    
    // Durakları say ve yazdır
    Durak* d = duraklar;
    int durak_sayisi = 0;
    while (d) {
        durak_sayisi++;
        d = d->sonraki;
    }
    printf("  ✓ %d durak yuklendi\n", durak_sayisi);
    
    printf("3. Hatlar yukleniyor...\n");
    Hat* hatlar = hatlariOku("data/hat.csv");
    if (!hatlar) {
        printf("HATA: Hatlar yuklenemedi!\n");
        return 1;
    }
    printf("  ✓ Hatlar yuklendi\n");
    
    // 3. GRAF OLUŞTUR
    printf("4. Graf olusturuluyor...\n");
    Graf* graf = grafOlustur(duraklar, hatlar);
    if (!graf) {
        printf("HATA: Graf olusturulamadi!\n");
        return 1;
    }
    printf("  ✓ Graf olusturuldu (%d dugum)\n", graf->dugum_sayisi);
    
    // 4. RAYLIB PENCERESİ
    printf("5. Raylib penceresi aciliyor...\n");
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Kent Ici Rota Planlayici");
    SetTargetFPS(60);
    
    // 5. GUI STATE
    printf("6. GUI baslatiliyor...\n");
    GuiState gui_state;
    gui_init(&gui_state, duraklar, hatlar, graf);
    printf("  ✓ GUI hazir\n");
    
    // 6. ANA DÖNGÜ
    printf("\n=== ANA DONGU BASLADI ===\n");
    printf("Butonlara tiklayip rota hesaplayabilirsiniz...\n");
    printf("Başlangıç ve hedef durakları seçin, sonra butona basın.\n");
    
    while (!WindowShouldClose()) {
        gui_update(&gui_state);
        
        BeginDrawing();
        ClearBackground(RAYWHITE);
        gui_draw(&gui_state);
        EndDrawing();
    }
    
    // 7. TEMİZLİK
    printf("\n=== PROGRAM SONLANIYOR ===\n");
    gui_cleanup(&gui_state);
    CloseWindow();
    
    if (graf) grafTemizle(graf);
    
    printf("Program basariyla sonlandi.\n");
    return 0;
}