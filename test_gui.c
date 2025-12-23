#include "raylib.h"

int main() {
    InitWindow(800, 600, "Raylib Test");
    SetTargetFPS(60);
    
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        
        DrawText("🎉 RAYLIB CALISIYOR!", 100, 100, 40, BLUE);
        DrawCircle(400, 300, 50, RED);
        
        EndDrawing();
    }
    
    CloseWindow();
    return 0;
}
