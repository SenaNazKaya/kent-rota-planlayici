#ifndef UI_H
#define UI_H

#include "durak.h"
#include "graf.h"
#include "bfs.h"
#include "dijkstra.h"

// Renk çiftleri
typedef enum {
    COLOR_PAIR_DEFAULT = 1,
    COLOR_PAIR_HEADER,
    COLOR_PAIR_MENU,
    COLOR_PAIR_SELECTED,
    COLOR_PAIR_SUCCESS,
    COLOR_PAIR_ERROR,
    COLOR_PAIR_INFO,
    COLOR_PAIR_ROTA
} ColorPair;

// Pencere yapısı
typedef struct {
    WINDOW* main_win;
    WINDOW* header_win;
    WINDOW* menu_win;
    WINDOW* content_win;
    WINDOW* footer_win;
} UIWindows;

// UI fonksiyonları
void init_ui();
void cleanup_ui();
void draw_header();
void draw_footer(const char* message);
void draw_main_menu(int selection);
void draw_loading();
void draw_durak_list(Graf* graf);
void draw_hat_list(Hat* hatlar);
void draw_graf_info(Graf* graf);
void draw_rota_bfs(Graf* graf, int bas_id, int hedef_id);
void draw_rota_dijkstra(Graf* graf, int bas_id, int hedef_id, int kriter);
void draw_message(const char* msg, ColorPair color);
int get_durak_input(Graf* graf, const char* prompt);
void wait_for_key();

#endif