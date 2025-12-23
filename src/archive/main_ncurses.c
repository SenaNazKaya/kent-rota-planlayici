#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    // NCurses başlat
    initscr();
    start_color();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    curs_set(0);
    
    // Renkleri tanımla
    init_pair(1, COLOR_CYAN, COLOR_BLACK);
    init_pair(2, COLOR_WHITE, COLOR_BLACK);
    init_pair(3, COLOR_BLACK, COLOR_WHITE);
    init_pair(4, COLOR_GREEN, COLOR_BLACK);
    init_pair(5, COLOR_RED, COLOR_BLACK);
    
    // Ekran boyutu
    int height, width;
    getmaxyx(stdscr, height, width);
    
    // Başlık
    clear();
    attron(COLOR_PAIR(1) | A_BOLD);
    mvprintw(2, (width-40)/2, "╔══════════════════════════════╗");
    mvprintw(3, (width-40)/2, "║   KENT İÇİ ROTA PLANLAYICI   ║");
    mvprintw(4, (width-40)/2, "║      (NCURSES VERSİYON)      ║");
    mvprintw(5, (width-40)/2, "╚══════════════════════════════╝");
    attroff(COLOR_PAIR(1) | A_BOLD);
    
    // Menü
    char* menu_items[] = {
        "Durakları Görüntüle",
        "Hatları Görüntüle",
        "Graf Yapısını Görüntüle",
        "BFS ile Rota Bul (En Az Aktarma)",
        "Dijkstra ile Rota Bul (En Kısa Mesafe)",
        "Dijkstra ile Rota Bul (En Kısa Süre)",
        "Çıkış"
    };
    
    int selection = 0;
    int ch;
    
    do {
        // Menüyü çiz
        for (int i = 0; i < 7; i++) {
            if (i == selection) {
                attron(COLOR_PAIR(3));
                mvprintw(8 + i, (width-40)/2, "  ▶ %s", menu_items[i]);
                attroff(COLOR_PAIR(3));
            } else {
                attron(COLOR_PAIR(2));
                mvprintw(8 + i, (width-40)/2, "    %s", menu_items[i]);
                attroff(COLOR_PAIR(2));
            }
        }
        
        // Alt bilgi
        attron(COLOR_PAIR(4));
        mvprintw(18, (width-50)/2, "↑↓: Seçim  Enter: Onayla  ESC: Çıkış");
        attroff(COLOR_PAIR(4));
        
        refresh();
        
        // Tuş girişi
        ch = getch();
        switch(ch) {
            case KEY_UP:
                selection = (selection - 1 + 7) % 7;
                break;
            case KEY_DOWN:
                selection = (selection + 1) % 7;
                break;
            case 10: // Enter
                if (selection == 6) {
                    ch = 27; // Çıkış
                } else {
                    // Seçilen öğeyi göster
                    clear();
                    attron(COLOR_PAIR(1) | A_BOLD);
                    mvprintw(5, (width-30)/2, "Seçilen: %s", menu_items[selection]);
                    attroff(COLOR_PAIR(1) | A_BOLD);
                    
                    attron(COLOR_PAIR(4));
                    mvprintw(10, (width-40)/2, "Bu özellik henüz eklenmedi!");
                    mvprintw(12, (width-40)/2, "Herhangi bir tuşa basın...");
                    attroff(COLOR_PAIR(4));
                    
                    refresh();
                    getch(); // Tuş bekle
                }
                break;
            case 27: // ESC
                selection = 6;
                ch = 10;
                break;
        }
        
    } while (ch != 27);
    
    // Çıkış
    clear();
    attron(COLOR_PAIR(1));
    mvprintw(10, (width-30)/2, "Program sonlandırılıyor...");
    attroff(COLOR_PAIR(1));
    refresh();
    usleep(1000000); // 1 saniye bekle
    
    endwin();
    
    printf("\nNCurses testi tamamlandı!\n");
    printf("CLI versiyonu çalıştırmak için: ./rota3\n");
    
    return 0;
}