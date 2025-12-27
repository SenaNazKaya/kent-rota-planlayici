#ifndef GUI_H
#define GUI_H

#include "/opt/homebrew/include/raylib.h"
#include <stdbool.h>

#include "../core/durak.h"
#include "../core/graf.h"
#include "../core/bfs.h"
#include "../core/dijkstra.h"

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 700
#define PANEL_WIDTH 300

#define COLOR_PANEL (Color){40, 44, 52, 255}
#define COLOR_BG (Color){240, 240, 245, 255}
#define COLOR_BUTTON (Color){70, 130, 180, 255}

typedef enum {
    PAGE_MAIN_MENU
} PageType;

typedef enum {
    INPUT_NONE
} InputMode;

typedef struct {
    PageType current_page;
    InputMode input_mode;

    int secili_durak;
    int baslangic_id;
    int hedef_id;

    bool rota_gosteriliyor;
    int son_dijkstra_kriter;
    int rota_turu;  
// 0 = BFS
// 1 = Dijkstra


    Durak* duraklar_list;
    Hat* hatlar_list;
    Graf* graf;

    BFSYol* bfs_yol;
    DijkstraYol* dijkstra_yol;
} GuiState;

/* GUI */
void gui_init(GuiState*, Durak*, Hat*, Graf*);
void gui_update(GuiState*);
void gui_draw(GuiState*);
void gui_cleanup(GuiState*);

/* Yardımcılar */
Vector2 gui_get_durak_position_by_id(GuiState*, int);
Color gui_hat_renk(const char*);

/* Rota yazımı */
void gui_rota_metni_ciz(GuiState*, int, int);
void gui_karsilastirma_ciz(GuiState*, int, int);

#endif
