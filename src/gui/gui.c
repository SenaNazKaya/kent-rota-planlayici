#include "gui.h"
#include <string.h>
#include <math.h>

/* ================= INIT ================= */
void gui_init(GuiState* state, Durak* duraklar, Hat* hatlar, Graf* graf) {
    memset(state, 0, sizeof(GuiState));

    state->baslangic_id = -1;
    state->hedef_id = -1;

    state->duraklar_list = duraklar;
    state->hatlar_list = hatlar;
    state->graf = graf;

    state->bfs_yol = NULL;
    state->dijkstra_yol = NULL;
    state->rota_gosteriliyor = false;
}

/* ================= HAT RENK ================= */
Color gui_hat_renk(const char* hat) {
    if (strstr(hat, "Metro"))  return BLUE;
    if (strstr(hat, "Otobus")) return ORANGE;
    if (strstr(hat, "Vapur"))  return PURPLE;
    return DARKGRAY;
}

/* ================= DURAK POZ ================= */
Vector2 gui_get_durak_position_by_id(GuiState* state, int id) {
    Durak* d = state->duraklar_list;
    int i = 0;
    while (d) {
        if (d->id == id) {
            return (Vector2){
                PANEL_WIDTH + 100 + (i % 5) * 120,
                120 + (i / 5) * 120
            };
        }
        d = d->sonraki;
        i++;
    }
    return (Vector2){-1, -1};
}

/* ================= UPDATE ================= */
void gui_update(GuiState* state) {
    Vector2 mouse = GetMousePosition();

    Rectangle bfs_btn = {50, 300, 200, 40};
    Rectangle dm_btn  = {50, 350, 200, 40};
    Rectangle ds_btn  = {50, 400, 200, 40};
    Rectangle temizle = {50, 450, 200, 40};

    /* 🔴 BUTONLAR ÖNCE */
    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {

        if (CheckCollisionPointRec(mouse, temizle)) {
            state->baslangic_id = -1;
            state->hedef_id = -1;
            state->rota_gosteriliyor = false;

            if (state->bfs_yol) bfsYolTemizle(state->bfs_yol);
            if (state->dijkstra_yol) dijkstraYolTemizle(state->dijkstra_yol);

            state->bfs_yol = NULL;
            state->dijkstra_yol = NULL;
            return;
        }

        if (state->baslangic_id != -1 && state->hedef_id != -1) {

            if (CheckCollisionPointRec(mouse, bfs_btn)) {
                if (state->bfs_yol) bfsYolTemizle(state->bfs_yol);
                state->bfs_yol = bfsEnKisaYol(
                    state->graf,
                    state->baslangic_id,
                    state->hedef_id
                );
                state->rota_turu = 0; // BFS
                state->rota_gosteriliyor = true;
                return;
            }

            if (CheckCollisionPointRec(mouse, dm_btn)) {
                if (state->dijkstra_yol) dijkstraYolTemizle(state->dijkstra_yol);
                state->dijkstra_yol = dijkstraEnKisaYol(
                    state->graf,
                    state->baslangic_id,
                    state->hedef_id,
                    0
                );
                state->rota_turu = 1; // Dijkstra
                state->son_dijkstra_kriter = 0; // mesafe
                state->rota_gosteriliyor = true;
                return;
            }

            if (CheckCollisionPointRec(mouse, ds_btn)) {
                if (state->dijkstra_yol) dijkstraYolTemizle(state->dijkstra_yol);
                state->dijkstra_yol = dijkstraEnKisaYol(
                    state->graf,
                    state->baslangic_id,
                    state->hedef_id,
                    1
                );
                state->rota_turu = 1; // Dijkstra
                state->son_dijkstra_kriter = 1; // sure
                state->rota_gosteriliyor = true;
                return;
            }
        }
    }

    /* 🟢 DURAK SEÇİMİ */
    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && mouse.x > PANEL_WIDTH) {
        Durak* d = state->duraklar_list;
        while (d) {
            Vector2 p = gui_get_durak_position_by_id(state, d->id);
            if (CheckCollisionPointCircle(mouse, p, 25)) {
                if (state->baslangic_id == -1) state->baslangic_id = d->id;
                else if (state->hedef_id == -1) state->hedef_id = d->id;
                break;
            }
            d = d->sonraki;
        }
    }
}

/* ================= DRAW ================= */
void gui_draw(GuiState* state) {

    /* SOL PANEL */
    DrawRectangle(0, 0, PANEL_WIDTH, SCREEN_HEIGHT, COLOR_PANEL);
    DrawText("ROTA PLANLAYICI", 40, 20, 24, WHITE);

    DrawText("1) Baslangic sec", 40, 80, 16, LIGHTGRAY);
    DrawText("2) Hedef sec",    40,105, 16, LIGHTGRAY);
    DrawText("3) Algoritma sec",40,130, 16, LIGHTGRAY);

    DrawRectangleRec((Rectangle){50,300,200,40}, COLOR_BUTTON);
    DrawText("BFS (Az Aktarma)", 55, 312, 14, WHITE);

    DrawRectangleRec((Rectangle){50,350,200,40}, COLOR_BUTTON);
    DrawText("Dijkstra Mesafe",  55, 362, 14, WHITE);

    DrawRectangleRec((Rectangle){50,400,200,40}, COLOR_BUTTON);
    DrawText("Dijkstra Sure",    55, 412, 14, WHITE);

    DrawRectangleRec((Rectangle){50,450,200,40}, GRAY);
    DrawText("Temizle", 115, 462, 14, WHITE);

    /* SAĞ PANEL */
    DrawRectangle(PANEL_WIDTH, 0,
        SCREEN_WIDTH - PANEL_WIDTH, SCREEN_HEIGHT, COLOR_BG);

    /* DURAKLAR */
    Durak* d = state->duraklar_list;
    while (d) {
        Vector2 p = gui_get_durak_position_by_id(state, d->id);
        Color c = BLUE;
        if (d->id == state->baslangic_id) c = GREEN;
        if (d->id == state->hedef_id) c = RED;

        DrawCircleV(p, 25, c);
        DrawCircleLines(p.x,p.y,25,BLACK);
        DrawText(d->ad, p.x-30, p.y-38, 10, BLACK);
        DrawText(TextFormat("ID:%d",d->id), p.x-18, p.y+28, 10, DARKGRAY);
        d = d->sonraki;
    }

    /* ================= ROTA + SONUÇ ================= */
    if (state->rota_gosteriliyor) {

        int *yol = NULL, len = 0;

        if (state->rota_turu == 0 && state->bfs_yol) {
            yol = state->bfs_yol->yol;
            len = state->bfs_yol->uzunluk;
        }
        if (state->rota_turu == 1 && state->dijkstra_yol) {
            yol = state->dijkstra_yol->yol;
            len = state->dijkstra_yol->uzunluk;
        }

        for (int i = 0; i < len - 1; i++) {
            int u = yol[i], v = yol[i+1];
            int idx = durakIDileIndex(state->graf, u);
            Hat* h = state->graf->kenarlar[idx];
            while (h) {
                if (h->hedef_id == v) {
                    Vector2 a = gui_get_durak_position_by_id(state, u);
                    Vector2 b = gui_get_durak_position_by_id(state, v);
                    DrawLineEx(a, b, 5, gui_hat_renk(h->hat_adi));
                    break;
                }
                h = h->sonraki;
            }
        }

        int x = PANEL_WIDTH + 30;
        int y = SCREEN_HEIGHT - 150;

        if (state->rota_turu == 0 && state->bfs_yol) {
            DrawText("BFS - EN AZ AKTARMA", x, y, 20, DARKGREEN);
            DrawText(TextFormat("Mesafe : %d km", state->bfs_yol->mesafe), x, y+30, 18, BLACK);
            DrawText(TextFormat("Sure    : %d dk", state->bfs_yol->sure),   x, y+55, 18, BLACK);
            DrawText(TextFormat("Aktarma : %d", state->bfs_yol->aktarma),  x, y+80, 18, MAROON);
        }

        if (state->rota_turu == 1 && state->dijkstra_yol) {
            DrawText(
                state->son_dijkstra_kriter == 0 ?
                "DIJKSTRA - EN KISA MESAFE" :
                "DIJKSTRA - EN KISA SURE",
                x, y, 20, DARKBLUE
            );
            DrawText(TextFormat("Mesafe : %d km", state->dijkstra_yol->mesafe), x, y+30, 18, BLACK);
            DrawText(TextFormat("Sure    : %d dk", state->dijkstra_yol->sure),   x, y+55, 18, BLACK);
            DrawText(TextFormat("Aktarma : %d", state->dijkstra_yol->aktarma),  x, y+80, 18, MAROON);
        }
    }
}

/* ================= CLEANUP ================= */
void gui_cleanup(GuiState* state) {
    if (state->bfs_yol) bfsYolTemizle(state->bfs_yol);
    if (state->dijkstra_yol) dijkstraYolTemizle(state->dijkstra_yol);
}
