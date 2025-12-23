#include "ui.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  // usleep için

static UIWindows windows;
static int screen_height, screen_width;

void init_ui() {
    // NCurses başlat
    initscr();
    start_color();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    curs_set(0);
    
    // Ekran boyutunu al
    getmaxyx(stdscr, screen_height, screen_width);
    
    // Minimum ekran kontrolü
    if (screen_height < 25 || screen_width < 80) {
        endwin();
        printf("Ekran boyutu en az 80x25 olmalı! (Mevcut: %dx%d)\n", 
               screen_width, screen_height);
        exit(1);
    }
    
    // Renk çiftlerini tanımla
    init_pair(COLOR_PAIR_DEFAULT, COLOR_WHITE, COLOR_BLACK);
    init_pair(COLOR_PAIR_HEADER, COLOR_CYAN, COLOR_BLACK);
    init_pair(COLOR_PAIR_MENU, COLOR_WHITE, COLOR_BLACK);
    init_pair(COLOR_PAIR_SELECTED, COLOR_BLACK, COLOR_WHITE);
    init_pair(COLOR_PAIR_SUCCESS, COLOR_GREEN, COLOR_BLACK);
    init_pair(COLOR_PAIR_ERROR, COLOR_RED, COLOR_BLACK);
    init_pair(COLOR_PAIR_INFO, COLOR_YELLOW, COLOR_BLACK);
    init_pair(COLOR_PAIR_ROTA, COLOR_MAGENTA, COLOR_BLACK);
    
    // Pencereleri oluştur
    windows.header_win = newwin(3, screen_width, 0, 0);
    windows.menu_win = newwin(screen_height - 6, screen_width / 2, 3, 0);
    windows.content_win = newwin(screen_height - 6, screen_width / 2, 3, screen_width / 2);
    windows.footer_win = newwin(3, screen_width, screen_height - 3, 0);
    
    // Pencere kutuları
    box(windows.header_win, 0, 0);
    box(windows.menu_win, 0, 0);
    box(windows.content_win, 0, 0);
    box(windows.footer_win, 0, 0);
    
    refresh();
}

void cleanup_ui() {
    // Pencereleri temizle
    delwin(windows.header_win);
    delwin(windows.menu_win);
    delwin(windows.content_win);
    delwin(windows.footer_win);
    
    // NCurses'i kapat
    endwin();
}

void draw_header() {
    wclear(windows.header_win);
    box(windows.header_win, 0, 0);
    
    wattron(windows.header_win, COLOR_PAIR(COLOR_PAIR_HEADER) | A_BOLD);
    mvwprintw(windows.header_win, 1, (screen_width - 40) / 2,
             "╔══════════════════════════════════╗");
    mvwprintw(windows.header_win, 2, (screen_width - 40) / 2,
             "║    KENT İÇİ ROTA PLANLAYICI     ║");
    mvwprintw(windows.header_win, 3, (screen_width - 40) / 2,
             "╚══════════════════════════════════╝");
    wattroff(windows.header_win, COLOR_PAIR(COLOR_PAIR_HEADER) | A_BOLD);
    
    wrefresh(windows.header_win);
}

void draw_footer(const char* message) {
    wclear(windows.footer_win);
    box(windows.footer_win, 0, 0);
    
    wattron(windows.footer_win, COLOR_PAIR(COLOR_PAIR_INFO));
    mvwprintw(windows.footer_win, 1, 2, "%s", message);
    wattroff(windows.footer_win, COLOR_PAIR(COLOR_PAIR_INFO));
    
    wrefresh(windows.footer_win);
}

void draw_loading() {
    wclear(windows.content_win);
    box(windows.content_win, 0, 0);
    
    wattron(windows.content_win, COLOR_PAIR(COLOR_PAIR_INFO) | A_BOLD);
    mvwprintw(windows.content_win, 5, 10, "⏳ Veriler yükleniyor...");
    wattroff(windows.content_win, COLOR_PAIR(COLOR_PAIR_INFO) | A_BOLD);
    
    // Animasyon
    for (int i = 0; i < 3; i++) {
        mvwprintw(windows.content_win, 5, 32 + i, ".");
        wrefresh(windows.content_win);
        usleep(300000); // 0.3 saniye
    }
    
    wrefresh(windows.content_win);
}