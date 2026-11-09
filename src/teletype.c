#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "message.h"

int main() {
    
    Msg* msg = calloc(1, sizeof(Msg));
    char* hello = "Hello World!\0";
    Msg_init(msg, strlen(hello), hello);
    Msg_print(msg);
    return 0;
    InitWindow(500,500, "Teletype");
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Hello from Raylib!", 200, 200, 12, GREEN);
        EndDrawing();
    }
}