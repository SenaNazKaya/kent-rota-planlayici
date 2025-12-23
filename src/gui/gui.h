#ifndef GUI_H
#define GUI_H

// MAC İÇİN DOĞRU YOL:
#include "/opt/homebrew/include/raylib.h"

#include <stdio.h>
#include "../core/durak.h"
#include "../core/graf.h"
#include "../core/bfs.h"
#include "../core/dijkstra.h"

// Ekran boyutları
#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 700
#define PANEL_WIDTH 300

// Renkler
#define COLOR_PANEL (Color){40, 44, 52, 255}
#define COLOR_BG (Color){240, 240, 245, 255}
#define COLOR_BUTTON (Color){70, 130, 180, 255}
#define COLOR_BUTTON_HOVER (Color){100, 160, 210, 255}
#define COLOR_TEXT (Color){230, 230, 230, 255}
#define COLOR_HIGHLIGHT (Color){255, 200, 50, 255}

// Sayfa tipleri
typedef enum {
    PAGE_MAIN_MENU,
    PAGE_DURAK_LIST,
    PAGE_HAT_LIST,
    PAGE_GRAF_VIEW,
    PAGE_BFS_INPUT,
    PAGE_DIJKSTRA_MESAFE_INPUT,
    PAGE_DIJKSTRA_SURE_INPUT,
    PAGE_ROTA_SONUC
} PageType;

// Giriş modu
typedef enum {
    INPUT_NONE,
    INPUT_BASLANGIC,
    INPUT_HEDEF
} InputMode;

// GUI durumu
typedef struct {
    PageType current_page;
    InputMode input_mode;
    
    int secili_menu_item;
    int secili_durak;
    int baslangic_id;
    int hedef_id;
    
    bool rota_hesaplaniyor;
    bool rota_gosteriliyor;
    
    // Veri yapıları
    Durak* duraklar_list;
    Hat* hatlar_list;
    Graf* graf;
    BFSYol* bfs_yol;
    DijkstraYol* dijkstra_yol;
    
    // Giriş için
    char input_buffer[10];
    int input_cursor;
    
    // Scroll için
    int scroll_offset;
    int max_scroll;
} GuiState;

// GUI fonksiyonları
void gui_init(GuiState* state, Durak* duraklar, Hat* hatlar, Graf* graf);
void gui_update(GuiState* state);
void gui_draw(GuiState* state);
void gui_cleanup(GuiState* state);

// Sayfa çizim fonksiyonları
void gui_draw_main_menu(GuiState* state);
void gui_draw_durak_list(GuiState* state);
void gui_draw_hat_list(GuiState* state);
void gui_draw_graf_view(GuiState* state);
void gui_draw_bfs_input(GuiState* state);
void gui_draw_dijkstra_input(GuiState* state, int kriter);
void gui_draw_rota_sonuc(GuiState* state);

// Yardımcı fonksiyonlar
void gui_draw_panel(const char* title);
void gui_draw_button(const char* text, int x, int y, int width, int height, bool hover);
void gui_draw_back_button();
int gui_handle_menu_input(GuiState* state);
void gui_handle_input_mode(GuiState* state);

#endif