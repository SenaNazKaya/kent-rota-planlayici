#include "raylib.h"
#include <stdio.h>

int main() {
    // Pencere aç
    InitWindow(800, 600, "Test Pencere");
    SetTargetFPS(60);
    
    printf("Pencere acildi! ESC ile cik.\n");
    
    while (!WindowShouldClose()) {
        // Çizim başla
        BeginDrawing();
        
        // Beyaz arkaplan
        ClearBackground(RAYWHITE);
        
        // Yazı yaz
        DrawText("MERHABA! RAYLIB CALISIYOR", 100, 100, 30, BLUE);
        
        // Daire çiz
        DrawCircle(400, 300, 50, RED);
        
        // Çizimi bitir
        EndDrawing();
    }
    
    // Pencereyi kapat
    CloseWindow();
    
    printf("Test tamamlandi.\n");
    return 0;
}
