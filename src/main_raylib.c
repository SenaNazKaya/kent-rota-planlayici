#include "raylib.h"
#include "core/durak.h"
#include "core/graf.h"
#include "gui/gui.h"
#include <stdio.h>

extern Durak* duraklariOku(const char* dosya_adi);
extern Hat* hatlariOku(const char* dosya_adi);
extern Graf* grafOlustur(Durak* duraklar, Hat* hatlar);
extern void grafTemizle(Graf* graf);

int main() {
    printf("Raylib Rota Planlayici baslatiliyor...\n");
    
    Durak* duraklar = duraklariOku("../data/durak.csv");
    Hat* hatlar = hatlariOku("../data/hat.csv");
    
    if (!duraklar || !hatlar) {
        printf("HATA: Veriler yuklenemedi!\n");
        return 1;
    }
    
    Graf* graf = grafOlustur(duraklar, hatlar);
    if (!graf) {
        printf("HATA: Graf olusturulamadi!\n");
        return 1;
    }
    
    printf("%d durak yuklendi.\n", graf->dugum_sayisi);
    
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Kent Ici Rota Planlayici");
    SetTargetFPS(60);
    
    GuiState gui_state;
    gui_init(&gui_state);
    
    while (!WindowShouldClose()) {
        gui_update(&gui_state);
        
        BeginDrawing();
        ClearBackground(RAYWHITE);
        gui_draw(&gui_state);
        EndDrawing();
    }
    
    gui_cleanup(&gui_state);
    CloseWindow();
    
    if (graf) grafTemizle(graf);
    
    printf("Program sonlandi.\n");
    return 0;
}
