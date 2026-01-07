#include "gui.h"
#include <string.h>
#include <stdio.h>
#include <math.h>  // sqrtf için

/* ================= INIT ================= */

void gui_init(GuiState* state, Durak* duraklar, Hat* hatlar, Graf* graf)
{
    memset(state, 0, sizeof(GuiState));

    state->baslangic_id = -1;
    state->hedef_id = -1;

    state->duraklar_list = duraklar;
    state->hatlar_list   = hatlar;
    state->graf          = graf;

    state->bfs_yol = NULL;
    state->dijkstra_mesafe = NULL;
    state->dijkstra_sure   = NULL;

    state->aktif_tab = TAB_NONE;
    state->rota_gosteriliyor = false;
}

/* ================= HAT RENK + IKON ================= */

Color gui_hat_renk(const char* hat)
{
    if (strstr(hat, "Marmaray")) return RED;
    if (strstr(hat, "Metro"))   return BLUE;
    if (strstr(hat, "Otobus"))  return ORANGE;
    if (strstr(hat, "Vapur"))   return PURPLE;
    return DARKGRAY;
}

int gui_hat_kalinlik(const char* hat)
{
    if (strstr(hat, "Marmaray")) return 8;
    if (strstr(hat, "Vapur"))    return 7;
    if (strstr(hat, "Metro"))    return 6;
    if (strstr(hat, "Otobus"))   return 5;
    return 4;
}

const char* gui_hat_ikon(const char* hat)
{
    if (strstr(hat, "Marmaray")) return "MR";
    if (strstr(hat, "Metro"))   return "M";
    if (strstr(hat, "Otobus"))  return "O";
    if (strstr(hat, "Vapur"))   return "V";
    return "?";
}

/* ================= DURAK POZISYON ================= */

Vector2 gui_get_durak_position_by_id(GuiState* state, int id)
{
    (void)state;

    switch (id) {
        // ÜST HAT
        case 1:  return (Vector2){480, 140};  // Taksim
        case 2:  return (Vector2){620, 140};  // Levent
        case 3:  return (Vector2){760, 140};  // Maslak
        case 4:  return (Vector2){900, 140}; // Kadıköy

        // ORTA HAT
        case 5:  return (Vector2){480, 300};  // Üsküdar
        case 6:  return (Vector2){620, 300};  // Beşiktaş
        case 7:  return (Vector2){760, 300};  // Şişli
        case 8:  return (Vector2){900, 300}; // Ataşehir

        // ALT HAT
        case 9:  return (Vector2){620, 460};  // Yenikapı
        case 10: return (Vector2){760, 460};  // Bostancı

        default: return (Vector2){-1, -1};
    }
}

/* ================= HAT BULMA YARDIMCI ================= */

static Hat* gui_find_hat_between(Graf* graf, int from_id, int to_id) {
    int from_idx = durakIDileIndex(graf, from_id);
    if (from_idx == -1) return NULL;
    
    Hat* h = graf->kenarlar[from_idx];
    while (h) {
        if (h->hedef_id == to_id) {
            return h;
        }
        h = h->sonraki;
    }
    return NULL;
}

/* ================= UPDATE ================= */

void gui_update(GuiState* state)
{
    Vector2 mouse = GetMousePosition();

    Rectangle bfs_btn = {50, 300, 200, 40};
    Rectangle dm_btn  = {50, 350, 200, 40};
    Rectangle ds_btn  = {50, 400, 200, 40};
    Rectangle temizle = {50, 450, 200, 40};

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        /* ================= TEMİZLE ================= */
        if (CheckCollisionPointRec(mouse, temizle))
        {
            state->baslangic_id = -1;
            state->hedef_id = -1;

            if (state->bfs_yol) {
                bfsYolTemizle(state->bfs_yol);
                state->bfs_yol = NULL;
            }

            if (state->dijkstra_mesafe) {
                dijkstraYolTemizle(state->dijkstra_mesafe);
                state->dijkstra_mesafe = NULL;
            }

            if (state->dijkstra_sure) {
                dijkstraYolTemizle(state->dijkstra_sure);
                state->dijkstra_sure = NULL;
            }

            state->aktif_tab = TAB_NONE;
            state->rota_gosteriliyor = false;
            return;
        }

        /* ================= BUTONLAR ================= */
        if (state->baslangic_id != -1 && state->hedef_id != -1)
        {
            /* BFS */
            if (CheckCollisionPointRec(mouse, bfs_btn))
            {
                if (state->bfs_yol)
                    bfsYolTemizle(state->bfs_yol);

                state->bfs_yol = bfsEnKisaYol(
                    state->graf,
                    state->baslangic_id,
                    state->hedef_id
                );

                state->aktif_tab = TAB_BFS;
                state->rota_gosteriliyor = true;
                return;
            }

            /* DIJKSTRA MESAFE */
            if (CheckCollisionPointRec(mouse, dm_btn))
            {
                if (state->dijkstra_mesafe)
                    dijkstraYolTemizle(state->dijkstra_mesafe);

                state->dijkstra_mesafe = dijkstraEnKisaYol(
                    state->graf,
                    state->baslangic_id,
                    state->hedef_id,
                    0  // Mesafe kriteri
                );

                state->aktif_tab = TAB_DIJKSTRA_MESAFE;
                state->rota_gosteriliyor = true;
                return;
            }

            /* DIJKSTRA SÜRE */
            if (CheckCollisionPointRec(mouse, ds_btn))
            {
                if (state->dijkstra_sure)
                    dijkstraYolTemizle(state->dijkstra_sure);

                state->dijkstra_sure = dijkstraEnKisaYol(
                    state->graf,
                    state->baslangic_id,
                    state->hedef_id,
                    1  // Süre kriteri
                );

                state->aktif_tab = TAB_DIJKSTRA_SURE;
                state->rota_gosteriliyor = true;
                return;
            }
        }

        /* ================= DURAK SEÇİMİ ================= */
        if (mouse.x > PANEL_WIDTH)
        {
            Durak* d = state->duraklar_list;
            while (d)
            {
                Vector2 p = gui_get_durak_position_by_id(state, d->id);
                if (CheckCollisionPointCircle(mouse, p, 25))
                {
                    if (state->baslangic_id == -1) {
                        state->baslangic_id = d->id;
                        printf("Başlangıç: %s (ID: %d)\n", d->ad, d->id);
                    }
                    else if (state->hedef_id == -1) {
                        state->hedef_id = d->id;
                        printf("Hedef: %s (ID: %d)\n", d->ad, d->id);
                    }
                    break;
                }
                d = d->sonraki;
            }
        }
    }
}

/* ================= DRAW ================= */
/* ================= DRAW ================= */

/* ================= DRAW ================= */

void gui_draw(GuiState* state)
{
    // Panel çiz
    DrawRectangle(0, 0, PANEL_WIDTH, SCREEN_HEIGHT, COLOR_PANEL);
    DrawText("ROTA PLANLAYICI", 40, 20, 24, WHITE);
    
    // Başlangıç ve hedef bilgileri
    char baslangic_text[50] = "Baslangic: -";
    char hedef_text[50] = "Hedef: -";
    
    if (state->baslangic_id != -1) {
        Durak* d = state->duraklar_list;
        while (d && d->id != state->baslangic_id) d = d->sonraki;
        if (d) snprintf(baslangic_text, 50, "Baslangic: %s", d->ad);
    }
    
    if (state->hedef_id != -1) {
        Durak* d = state->duraklar_list;
        while (d && d->id != state->hedef_id) d = d->sonraki;
        if (d) snprintf(hedef_text, 50, "Hedef: %s", d->ad);
    }
    
    DrawText(baslangic_text, 50, 80, 16, WHITE);
    DrawText(hedef_text, 50, 100, 16, WHITE);
    
    // Butonlar
    DrawRectangleRec((Rectangle){50, 300, 200, 40}, COLOR_BUTTON);
    DrawText("BFS (Az Aktarma)", 55, 312, 14, WHITE);
    
    DrawRectangleRec((Rectangle){50, 350, 200, 40}, COLOR_BUTTON);
    DrawText("Dijkstra Mesafe", 55, 362, 14, WHITE);
    
    DrawRectangleRec((Rectangle){50, 400, 200, 40}, COLOR_BUTTON);
    DrawText("Dijkstra Sure", 55, 412, 14, WHITE);
    
    DrawRectangleRec((Rectangle){50, 450, 200, 40}, GRAY);
    DrawText("Temizle", 115, 462, 14, WHITE);
    
    // Ana ekran
    DrawRectangle(PANEL_WIDTH, 0, SCREEN_WIDTH - PANEL_WIDTH, SCREEN_HEIGHT, COLOR_BG);
    
    // Durakları çiz
    Durak* d = state->duraklar_list;
    while (d)
    {
        Vector2 p = gui_get_durak_position_by_id(state, d->id);
        Color c = (d->id == state->baslangic_id) ? GREEN :
                  (d->id == state->hedef_id)    ? RED   : BLUE;
        
        DrawCircleV(p, 25, c);
        DrawCircleLines(p.x, p.y, 25, BLACK);
        DrawText(d->ad, p.x - MeasureText(d->ad, 10)/2, p.y + 32, 10, BLACK);

        
        d = d->sonraki;
    }
    
    // Rota bilgisi göster
    if (state->rota_gosteriliyor && state->aktif_tab != TAB_NONE) {
        int *yol = NULL;
        int len = 0;
        int mesafe = 0;
        int sure = 0;
        int aktarma = 0;
        
        if (state->aktif_tab == TAB_BFS && state->bfs_yol) {
            yol = state->bfs_yol->yol;
            len = state->bfs_yol->uzunluk;
            mesafe = state->bfs_yol->mesafe;
            sure = state->bfs_yol->sure;
            aktarma = state->bfs_yol->aktarma;
        }
        else if (state->aktif_tab == TAB_DIJKSTRA_MESAFE && state->dijkstra_mesafe) {
            yol = state->dijkstra_mesafe->yol;
            len = state->dijkstra_mesafe->uzunluk;
            mesafe = state->dijkstra_mesafe->mesafe;
            sure = state->dijkstra_mesafe->sure;
            aktarma = state->dijkstra_mesafe->aktarma;
        }
        else if (state->aktif_tab == TAB_DIJKSTRA_SURE && state->dijkstra_sure) {
            yol = state->dijkstra_sure->yol;
            len = state->dijkstra_sure->uzunluk;
            mesafe = state->dijkstra_sure->mesafe;
            sure = state->dijkstra_sure->sure;
            aktarma = state->dijkstra_sure->aktarma;
        }
        
        if (yol && len > 0) {
            // Önce tüm hatları çiz
            for (int i = 0; i < len - 1; i++) {
                int from_id = yol[i];
                int to_id = yol[i + 1];
                
                Vector2 a = gui_get_durak_position_by_id(state, from_id);
                Vector2 b = gui_get_durak_position_by_id(state, to_id);
                
                Hat* h = gui_find_hat_between(state->graf, from_id, to_id);
                
                if (h) {
                    // Çizgiyi çiz
                    DrawLineEx(a, b, gui_hat_kalinlik(h->hat_adi), gui_hat_renk(h->hat_adi));
                    
                    // Ok başı çiz
                    Vector2 direction = {b.x - a.x, b.y - a.y};
                    float length = sqrtf(direction.x * direction.x + direction.y * direction.y);
                    if (length > 0) {
                        direction.x /= length;
                        direction.y /= length;
                        
                        Vector2 arrow1 = {b.x - direction.x * 15 - direction.y * 5, 
                                         b.y - direction.y * 15 + direction.x * 5};
                        Vector2 arrow2 = {b.x - direction.x * 15 + direction.y * 5, 
                                         b.y - direction.y * 15 - direction.x * 5};
                        
                        DrawLineEx(b, arrow1, 3, gui_hat_renk(h->hat_adi));
                        DrawLineEx(b, arrow2, 3, gui_hat_renk(h->hat_adi));
                    }
                    
                    // Çizginin ortasına ikon ekle
                    Vector2 midpoint = {(a.x + b.x) / 2, (a.y + b.y) / 2};
                    const char* ikon = gui_hat_ikon(h->hat_adi);
                    
                    // İkon için arkaplan
                    DrawCircleV(midpoint, 12, WHITE);
                    DrawCircleLines(midpoint.x, midpoint.y, 12, BLACK);
                    
                    // İkon metni
                    DrawText(ikon, midpoint.x - MeasureText(ikon, 10)/2, 
                            midpoint.y - 5, 10, BLACK);
                } else {
                    DrawLineEx(a, b, 4, BLACK);
                }
            }
            
            // Bilgi paneli
            char info[200];
            const char* algo_name = "";
            if (state->aktif_tab == TAB_BFS) algo_name = "BFS";
            else if (state->aktif_tab == TAB_DIJKSTRA_MESAFE) algo_name = "Dijkstra (Mesafe)";
            else algo_name = "Dijkstra (Sure)";
            
            snprintf(info, sizeof(info), "%s\nDurak: %d | Mesafe: %d km | Sure: %d dak | Aktarma: %d", 
                    algo_name, len, mesafe, sure, aktarma);
            
            DrawRectangle(30, 500, 260, 140, (Color){0, 0, 0, 180});
            DrawText(info, 40, 510, 12, WHITE);
            
            // Yolu listele
            char yol_text[500] = "Yol: ";
            int pos = 5;
            for (int i = 0; i < len; i++) {
                Durak* dur = state->duraklar_list;
                while (dur && dur->id != yol[i]) dur = dur->sonraki;
                if (dur) {
                    pos += snprintf(yol_text + pos, sizeof(yol_text) - pos, "%s", dur->ad);
                    if (i < len - 1) {
                        pos += snprintf(yol_text + pos, sizeof(yol_text) - pos, " -> ");
                    }
                }
            }
            
            DrawText(yol_text, 40, 620, 10, WHITE);
        }
    }
    
    // ULAŞIM ARAÇLARI GÖSTERGESİ (LEGEND) - Her zaman görünür - SAĞ ALTA
    int legend_x = SCREEN_WIDTH - 210; // Sağdan 10 piksel boşluk
    int legend_y = SCREEN_HEIGHT - 160; // Alttan 20 piksel boşluk
    int legend_width = 200;
    int legend_height = 150;
    
    // Gösterge arkaplanı
    DrawRectangle(legend_x, legend_y, legend_width, legend_height, (Color){0, 0, 0, 200});
    DrawRectangleLines(legend_x, legend_y, legend_width, legend_height, WHITE);
    DrawText("Ulasim Araclari", legend_x + 10, legend_y + 10, 16, WHITE);
    
    // Çizgi örnekleri (sol tarafta renkli çizgiler)
    int line_x = legend_x + 15;
    int text_x = legend_x + 45;
    int line_y_start = legend_y + 40;
    int line_spacing = 25;
    
    // Marmaray
    DrawLine(line_x, line_y_start, line_x + 25, line_y_start, RED);
    DrawLineEx((Vector2){line_x, line_y_start}, (Vector2){line_x + 25, line_y_start}, 8, RED);
    DrawText("Marmaray (MR)", text_x, line_y_start - 6, 12, WHITE);
    
    // Metro
    DrawLine(line_x, line_y_start + line_spacing, line_x + 25, line_y_start + line_spacing, BLUE);
    DrawLineEx((Vector2){line_x, line_y_start + line_spacing}, 
               (Vector2){line_x + 25, line_y_start + line_spacing}, 6, BLUE);
    DrawText("Metro (M)", text_x, line_y_start + line_spacing - 6, 12, WHITE);
    
    // Otobüs
    DrawLine(line_x, line_y_start + 2*line_spacing, line_x + 25, line_y_start + 2*line_spacing, ORANGE);
    DrawLineEx((Vector2){line_x, line_y_start + 2*line_spacing}, 
               (Vector2){line_x + 25, line_y_start + 2*line_spacing}, 5, ORANGE);
    DrawText("Otobus (O)", text_x, line_y_start + 2*line_spacing - 6, 12, WHITE);
    
    // Vapur
    DrawLine(line_x, line_y_start + 3*line_spacing, line_x + 25, line_y_start + 3*line_spacing, PURPLE);
    DrawLineEx((Vector2){line_x, line_y_start + 3*line_spacing}, 
               (Vector2){line_x + 25, line_y_start + 3*line_spacing}, 7, PURPLE);
    DrawText("Vapur (V)", text_x, line_y_start + 3*line_spacing - 6, 12, WHITE);
}

/* ================= CLEANUP ================= */

void gui_cleanup(GuiState* state)
{
    if (state->bfs_yol) bfsYolTemizle(state->bfs_yol);
    if (state->dijkstra_mesafe) dijkstraYolTemizle(state->dijkstra_mesafe);
    if (state->dijkstra_sure) dijkstraYolTemizle(state->dijkstra_sure);
}