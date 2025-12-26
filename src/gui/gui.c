#include "gui.h"
#include <math.h>
#include <string.h>

void gui_init(GuiState* state, Durak* duraklar, Hat* hatlar, Graf* graf) {
    state->current_page = PAGE_MAIN_MENU;
    state->input_mode = INPUT_NONE;
    state->secili_menu_item = 0;
    state->secili_durak = -1;
    state->baslangic_id = -1;
    state->hedef_id = -1;
    state->rota_hesaplaniyor = false;
    state->rota_gosteriliyor = false;
    
    state->duraklar_list = duraklar;
    state->hatlar_list = hatlar;
    state->graf = graf;
    state->bfs_yol = NULL;
    state->dijkstra_yol = NULL;
    
    memset(state->input_buffer, 0, sizeof(state->input_buffer));
    state->input_cursor = 0;
    state->scroll_offset = 0;
    state->max_scroll = 0;
}


int gui_get_durak_from_mouse(Vector2 mouse) {
    int durak_positions[5][2] = {
        {400, 200},
        {500, 300},
        {600, 200},
        {700, 300},
        {800, 200}
    };
    
    for (int i = 0; i < 5; i++) {
        float dx = mouse.x - durak_positions[i][0];
        float dy = mouse.y - durak_positions[i][1];
        float distance = sqrt(dx*dx + dy*dy);
        if (distance < 25) return i;
    }
    return -1;
}

void gui_update(GuiState* state) {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mouse = GetMousePosition();
        
        if (mouse.x > PANEL_WIDTH) {
            int clicked_durak = gui_get_durak_from_mouse(mouse);
            if (clicked_durak != -1) {
                state->secili_durak = clicked_durak;
                if (state->baslangic_id == -1) {
                    state->baslangic_id = clicked_durak;
                } else if (state->hedef_id == -1) {
                    state->hedef_id = clicked_durak;
                }
            }
        }
        
        Rectangle rota_btn = {50, 200, 200, 50};
        Rectangle temizle_btn = {50, 270, 200, 50};
        
        if (CheckCollisionPointRec(mouse, rota_btn)) {
            if (state->baslangic_id != -1 && state->hedef_id != -1) {
                state->rota_hesaplaniyor = true;
            }
        }
        
        if (CheckCollisionPointRec(mouse, temizle_btn)) {
            state->baslangic_id = -1;
            state->hedef_id = -1;
            state->secili_durak = -1;
        }
    }
}

void gui_draw(GuiState* state) {
    DrawRectangle(0, 0, PANEL_WIDTH, SCREEN_HEIGHT, COLOR_PANEL);
    DrawText("ROTA PLANLAYICI", 30, 20, 24, WHITE);
    
    if (state->secili_durak != -1) {
        DrawText(TextFormat("Secili: ID %d", state->secili_durak), 30, 100, 20, YELLOW);
    }
    if (state->baslangic_id != -1) {
        DrawText(TextFormat("Baslangic: ID %d", state->baslangic_id), 30, 130, 18, GREEN);
    }
    if (state->hedef_id != -1) {
        DrawText(TextFormat("Hedef: ID %d", state->hedef_id), 30, 160, 18, RED);
    }
    
    Rectangle rota_btn = {50, 200, 200, 50};
    Rectangle temizle_btn = {50, 270, 200, 50};
    DrawRectangleRec(rota_btn, COLOR_BUTTON);
    DrawText("ROTA BUL", 100, 215, 24, WHITE);
    DrawRectangleRec(temizle_btn, GRAY);
    DrawText("TEMIZLE", 100, 285, 24, WHITE);
    
    DrawRectangle(PANEL_WIDTH, 0, SCREEN_WIDTH - PANEL_WIDTH, SCREEN_HEIGHT, COLOR_BG);
    
    int durak_positions[5][2] = {{400,200},{500,300},{600,200},{700,300},{800,200}};
    
    for (int i = 0; i < 5; i++) {
        Color renk = BLUE;
        if (i == state->baslangic_id) renk = GREEN;
        if (i == state->hedef_id) renk = RED;
        
        DrawCircle(durak_positions[i][0], durak_positions[i][1], 25, renk);
        DrawCircleLines(durak_positions[i][0], durak_positions[i][1], 25, BLACK);
        DrawText(TextFormat("%d", i), durak_positions[i][0]-5, durak_positions[i][1]-7, 15, WHITE);
    }
    
    for (int i = 0; i < 4; i++) {
        DrawLine(durak_positions[i][0], durak_positions[i][1],
                durak_positions[i+1][0], durak_positions[i+1][1], DARKGRAY);
    }
    
    DrawText("ESC: Cikis | Sol tik: Durak sec", 30, SCREEN_HEIGHT-40, 15, LIGHTGRAY);
    
    if (state->rota_hesaplaniyor) {
        DrawText("Rota hesaplaniyor...", SCREEN_WIDTH/2-100, 50, 20, DARKBLUE);
    }
}

void gui_cleanup(GuiState* state) {
}
