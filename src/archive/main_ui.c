#include "durak.h"
#include "graf.h"
#include "bfs.h"
#include "dijkstra.h"
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <locale.h>

extern Durak* duraklariOku(const char* dosya_adi);
extern Hat* hatlariOku(const char* dosya_adi);
extern Graf* grafOlustur(Durak* duraklar, Hat* hatlar);
extern void grafTemizle(Graf* graf);

// Renk çiftleri
#define COLOR_PAIR_DEFAULT 1
#define COLOR_PAIR_HEADER 2
#define COLOR_PAIR_MENU 3
#define COLOR_PAIR_SELECTED 4
#define COLOR_PAIR_SUCCESS 5
#define COLOR_PAIR_ERROR 6
#define COLOR_PAIR_INFO 7

WINDOW *header_win, *menu_win, *content_win, *footer_win;
int screen_height, screen_width;

void init_ui() {
    setlocale(LC_ALL, "en_US.UTF-8");
    
    initscr();
    start_color();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    curs_set(0);
    
    // MOUSE DESTEĞİ
    mousemask(ALL_MOUSE_EVENTS, NULL);
    
    getmaxyx(stdscr, screen_height, screen_width);
    
    init_pair(COLOR_PAIR_DEFAULT, COLOR_WHITE, COLOR_BLACK);
    init_pair(COLOR_PAIR_HEADER, COLOR_CYAN, COLOR_BLACK);
    init_pair(COLOR_PAIR_MENU, COLOR_WHITE, COLOR_BLACK);
    init_pair(COLOR_PAIR_SELECTED, COLOR_BLACK, COLOR_CYAN); // Mavi arka plan
    init_pair(COLOR_PAIR_SUCCESS, COLOR_GREEN, COLOR_BLACK);
    init_pair(COLOR_PAIR_ERROR, COLOR_RED, COLOR_BLACK);
    init_pair(COLOR_PAIR_INFO, COLOR_YELLOW, COLOR_BLACK);
    
    header_win = newwin(3, screen_width, 0, 0);
    menu_win = newwin(screen_height - 6, screen_width / 2, 3, 0);
    content_win = newwin(screen_height - 6, screen_width / 2, 3, screen_width / 2);
    footer_win = newwin(3, screen_width, screen_height - 3, 0);
    
    box(header_win, 0, 0);
    box(menu_win, 0, 0);
    box(content_win, 0, 0);
    box(footer_win, 0, 0);
    
    refresh();
}

void cleanup_ui() {
    delwin(header_win);
    delwin(menu_win);
    delwin(content_win);
    delwin(footer_win);
    endwin();
}

void draw_header() {
    wclear(header_win);
    box(header_win, 0, 0);
    
    wattron(header_win, COLOR_PAIR(COLOR_PAIR_HEADER) | A_BOLD);
    mvwprintw(header_win, 1, (screen_width - 40) / 2, "🚇 KENT ICI ROTA PLANLAYICI 🚇");
    wattroff(header_win, COLOR_PAIR(COLOR_PAIR_HEADER) | A_BOLD);
    
    wrefresh(header_win);
}

void draw_footer(const char* message) {
    wclear(footer_win);
    box(footer_win, 0, 0);
    mvwprintw(footer_win, 1, 2, "🖱️ %s", message);
    wrefresh(footer_win);
}

void draw_main_menu(int selection) {
    wclear(menu_win);
    box(menu_win, 0, 0);
    
    char* menu_items[] = {
        "📋 Duraklari Listele",
        "🛤️  Hatlari Listele", 
        "🗺️  Graf Yapisini Gor",
        "🔍 BFS ile Rota Bul",
        "📏 Dijkstra (Mesafe)",
        "⏱️  Dijkstra (Sure)",
        "🚪 Cikis"
    };
    
    for (int i = 0; i < 7; i++) {
        if (i == selection) {
            wattron(menu_win, COLOR_PAIR(COLOR_PAIR_SELECTED) | A_BOLD);
            mvwprintw(menu_win, i + 3, 4, "▶ %s", menu_items[i]);
            wattroff(menu_win, COLOR_PAIR(COLOR_PAIR_SELECTED) | A_BOLD);
        } else {
            wattron(menu_win, COLOR_PAIR(COLOR_PAIR_MENU));
            mvwprintw(menu_win, i + 3, 6, "  %s", menu_items[i]);
            wattroff(menu_win, COLOR_PAIR(COLOR_PAIR_MENU));
        }
    }
    
    wrefresh(menu_win);
}

void show_message(const char* msg, int color_pair) {
    wclear(content_win);
    box(content_win, 0, 0);
    
    wattron(content_win, COLOR_PAIR(color_pair) | A_BOLD);
    mvwprintw(content_win, 5, 10, "%s", msg);
    wattroff(content_win, COLOR_PAIR(color_pair) | A_BOLD);
    
    wrefresh(content_win);
}

void wait_for_key() {
    draw_footer("Herhangi bir tusa basin...");
    wgetch(footer_win);
}

void display_durak_list(Graf* graf) {
    wclear(content_win);
    box(content_win, 0, 0);
    
    wattron(content_win, COLOR_PAIR(COLOR_PAIR_HEADER));
    mvwprintw(content_win, 1, 2, "📋 DURAK LISTESI");
    wattroff(content_win, COLOR_PAIR(COLOR_PAIR_HEADER));
    
    int y = 3;
    for (int i = 0; i < graf->dugum_sayisi && y < screen_height - 10; i++) {
        mvwprintw(content_win, y++, 4, "%d. %s (ID: %d)", 
                 i + 1, graf->duraklar[i]->ad, graf->duraklar[i]->id);
    }
    
    wrefresh(content_win);
}

int main() {
    // Arayüzü başlat
    init_ui();
    draw_header();
    draw_footer("↑↓ veya 🖱️: Secim | Enter: Onayla | ESC: Cikis");
    
    // Verileri yükle
    show_message("📥 Veriler yukleniyor...", COLOR_PAIR_INFO);
    usleep(500000);
    
    Durak* duraklar = duraklariOku("../data/durak.csv");
    Hat* hatlar = hatlariOku("../data/hat.csv");
    
    if (!duraklar || !hatlar) {
        show_message("❌ HATA: Veriler yuklenemedi!", COLOR_PAIR_ERROR);
        wait_for_key();
        cleanup_ui();
        return 1;
    }
    
    Graf* graf = grafOlustur(duraklar, hatlar);
    if (!graf) {
        show_message("❌ HATA: Graf olusturulamadi!", COLOR_PAIR_ERROR);
        wait_for_key();
        cleanup_ui();
        return 1;
    }
    
    show_message("✅ Veriler basariyla yuklendi!", COLOR_PAIR_SUCCESS);
    usleep(800000);
    
    // Ana menü döngüsü
    int selection = 0;
    int ch;
    
    do {
        draw_main_menu(selection);
        
        ch = getch();
        
        // MOUSE KONTROLÜ
        if (ch == KEY_MOUSE) {
            MEVENT event;
            if (getmouse(&event) == OK) {
                // Sadece tıklama
                if (event.bstate & BUTTON1_CLICKED) {
                    // Menu penceresinde mi?
                    if (event.x >= 0 && event.x < screen_width/2 &&
                        event.y >= 3 && event.y < screen_height - 3) {
                        
                        int clicked_item = event.y - 3 - 2; // Offset hesapla
                        if (clicked_item >= 0 && clicked_item < 7) {
                            selection = clicked_item;
                            ch = 10; // Enter gibi davran
                        }
                    }
                }
            }
            if (ch != 10) continue; // Enter değilse devam et
        }
        
        switch(ch) {
            case KEY_UP:
                selection = (selection - 1 + 7) % 7;
                break;
            case KEY_DOWN:
                selection = (selection + 1) % 7;
                break;
            case 10: // Enter
                if (selection == 6) { // Çıkış
                    ch = 27;
                } else if (selection == 0) { // Duraklar
                    display_durak_list(graf);
                    wait_for_key();
                } else if (selection == 3) { // BFS
                    show_message("🔍 BFS - Gelistiriliyor...", COLOR_PAIR_INFO);
                    wait_for_key();
                } else if (selection == 4) { // Dijkstra mesafe
                    show_message("📏 Dijkstra - Gelistiriliyor...", COLOR_PAIR_INFO);
                    wait_for_key();
                } else if (selection == 5) { // Dijkstra süre
                    show_message("⏱️ Dijkstra - Gelistiriliyor...", COLOR_PAIR_INFO);
                    wait_for_key();
                } else {
                    show_message("⏳ Bu ozellik gelistiriliyor...", COLOR_PAIR_INFO);
                    wait_for_key();
                }
                break;
            case 27: // ESC
                selection = 6;
                ch = 10;
                break;
        }
        
    } while (ch != 27);
    
    // Çıkış
    show_message("👋 Program sonlandiriliyor...", COLOR_PAIR_INFO);
    usleep(500000);
    
    // Temizlik
    grafTemizle(graf);
    
    Durak* temp_durak;
    while (duraklar) {
        temp_durak = duraklar;
        duraklar = duraklar->sonraki;
        free(temp_durak);
    }
    
    Hat* temp_hat;
    while (hatlar) {
        temp_hat = hatlar;
        hatlar = hatlar->sonraki;
        free(temp_hat);
    }
    
    cleanup_ui();
    printf("\n✅ Program basariyla sonlandi.\n");
    printf("📁 CLI versiyonu icin: ./rota3\n");
    
    return 0;
}