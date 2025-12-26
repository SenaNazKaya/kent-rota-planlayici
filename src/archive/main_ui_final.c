#include "durak.h"
#include "graf.h"
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>

extern Durak* duraklariOku(const char* dosya_adi);
extern Hat* hatlariOku(const char* dosya_adi);
extern Graf* grafOlustur(Durak* duraklar, Hat* hatlar);
extern void grafTemizle(Graf* graf);

int main() {
    printf("TUI baslatiliyor...\n");
    
    // 1. NCurses başlat
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    
    // 2. Verileri yükle (TERMİNAL'DE debug)
    printw("Veriler yukleniyor...\n");
    refresh();
    
    Durak* duraklar = duraklariOku("data/durak.csv");
    Hat* hatlar = hatlariOku("data/hat.csv");
    
    if (!duraklar || !hatlar) {
        printw("HATA: Veriler yuklenemedi!\n");
        refresh();
        getch();
        endwin();
        return 1;
    }
    
    Graf* graf = grafOlustur(duraklar, hatlar);
    if (!graf) {
        printw("HATA: Graf olusturulamadi!\n");
        refresh();
        getch();
        endwin();
        return 1;
    }
    
    // 3. BAŞARILI - Menüyü göster
    clear();
    mvprintw(1, 10, "=== KENT ROTA PLANLAYICI ===");
    mvprintw(2, 10, "✅ Veriler yuklendi: %d durak", graf->dugum_sayisi);
    
    mvprintw(4, 10, "1. Duraklari Listele");
    mvprintw(5, 10, "2. Hatlari Listele");
    mvprintw(6, 10, "3. Rota Bul (BFS)");
    mvprintw(7, 10, "4. Rota Bul (Dijkstra)");
    mvprintw(8, 10, "ESC. Cikis");
    mvprintw(10, 10, "Secim: ");
    refresh();
    
    int ch = getch();
    
    if (ch == '1') {
        clear();
        mvprintw(1, 10, "=== DURAKLAR (%d adet) ===", graf->dugum_sayisi);
        
        for (int i = 0; i < graf->dugum_sayisi && i < 15; i++) {
            if (graf->duraklar[i]) {
                mvprintw(3 + i, 5, "%d. %s (ID: %d)", 
                        i+1, graf->duraklar[i]->ad, graf->duraklar[i]->id);
            }
        }
        
        mvprintw(20, 10, "Herhangi tusa basin...");
        refresh();
        getch();
    }
    
    // 4. Temizlik
    grafTemizle(graf);
    endwin();
    
    printf("\nTUI basariyla sonlandi. %d durak islendi.\n", graf->dugum_sayisi);
    return 0;
}
