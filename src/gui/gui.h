#ifndef GUI_H
#define GUI_H

// ===== macOS ncurses / raylib KEY çakışması çözümü =====
#ifdef KEY_ENTER
#undef KEY_ENTER
#endif
#ifdef KEY_BACKSPACE
#undef KEY_BACKSPACE
#endif
#ifdef KEY_LEFT
#undef KEY_LEFT
#endif
#ifdef KEY_RIGHT
#undef KEY_RIGHT
#endif
#ifdef KEY_UP
#undef KEY_UP
#endif
#ifdef KEY_DOWN
#undef KEY_DOWN
#endif
#ifdef KEY_HOME
#undef KEY_HOME
#endif
#ifdef KEY_END
#undef KEY_END
#endif

#include "/opt/homebrew/include/raylib.h"
#include <stdbool.h>

#include "../core/durak.h"
#include "../core/graf.h"
#include "../core/bfs.h"
#include "../core/dijkstra.h"

/* ================= EKRAN ================= */

#define SCREEN_WIDTH  1000
#define SCREEN_HEIGHT 700
#define PANEL_WIDTH   320

#define COLOR_PANEL  (Color){40, 44, 52, 255}
#define COLOR_BG     (Color){240, 240, 245, 255}
#define COLOR_BUTTON (Color){70, 130, 180, 255}

/* ================= HAT RENKLERİ ================= */

#define COLOR_METRO     (Color){ 80, 160, 255, 255 }
#define COLOR_MARMARAY  (Color){220,  40,  40, 255 }   // 🔴 Marmaray
#define COLOR_VAPUR     (Color){ 60, 180, 120, 255 }
#define COLOR_OTOBUS    (Color){255, 140,   0, 255 }
#define COLOR_DEFAULT   (Color){120, 120, 120, 255 }

/* ================= ROTA SEKME ================= */

typedef enum {
    TAB_NONE = -1,
    TAB_BFS = 0,
    TAB_DIJKSTRA_MESAFE = 1,
    TAB_DIJKSTRA_SURE = 2
} RotaTab;

/* ================= GUI STATE ================= */

typedef struct {

    int baslangic_id;
    int hedef_id;

    bool rota_gosteriliyor;
    RotaTab aktif_tab;

    Durak* duraklar_list;
    Hat*   hatlar_list;
    Graf*  graf;

    BFSYol* bfs_yol;
    DijkstraYol* dijkstra_mesafe;
    DijkstraYol* dijkstra_sure;

} GuiState;

/* ================= GUI ================= */

void gui_init(GuiState*, Durak*, Hat*, Graf*);
void gui_update(GuiState*);
void gui_draw(GuiState*);
void gui_cleanup(GuiState*);

/* ================= YARDIMCILAR ================= */

Vector2 gui_get_durak_position_by_id(GuiState*, int);

/* HAT RENK + İKON */
Color gui_hat_renk(const char* hat_adi);
const char* gui_hat_ikon(const char* hat_adi);

/* LEGENDE (Metro / Marmaray / Vapur / Otobüs) */
void gui_ulasim_legend_ciz(void);

#endif
