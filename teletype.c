#include <raylib.h>
#include <stdio.h>

int main() {
    InitWindow(500,500, "Teletype");

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Hello from Raylib!", 200, 200, 12, GREEN);
        EndDrawing();
    }

    
}