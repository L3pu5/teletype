#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "bindings.h"
#include "message.h"

void handle_user_input(ctx* ctx) {
    if (IsKeyPressed(KEY_DOWN)) {
        Text_advance(ctx->text, 1);
    }

    if (IsKeyPressed(KEY_UP)) {
        Text_advance(ctx->text, -1);
    }
}

int main(int argc, char** argv) {
    if(argc == 1) {
        printf("Usage: \n");
        printf("\t\tteletpye <file.txt>\n");
        exit(0);
    }

    Msg* msg = calloc(1, sizeof(Msg));

    Text lorem = {0};
    Text_init(&lorem, TEXT_WIND_WINDOW_DEFALT);
    Text_open_file(&lorem, argv[1]);

    Feed loremFeed = {0};
    Feed_init(&loremFeed, &lorem);
    Feed_construct(&loremFeed, &lorem);

    lorem.feed = &loremFeed;

    // Context
    ctx ctx = {0};
    Ctx_init_default(&ctx);
    ctx.text = &lorem;
    Ctx_calcualte_coundown_time_from_message(&ctx, &ctx.text->feed->messages[ctx.text->feed->cursor]);

    // TextWindow
    TextWindow window = {0};
    window.height = 500;
    window.offsetX = 0;
    window.offsetY = 0;
    window.width = 300;

    float time = 0.0f;

    InitWindow(300,500, "Teletype");
    SetWindowState(FLAG_WINDOW_TOPMOST);
    SetWindowState(FLAG_WINDOW_UNDECORATED);
    SetWindowState(FLAG_WINDOW_TRANSPARENT);
    while (!WindowShouldClose()) {
        handle_user_input(&ctx);

        BeginDrawing();
        ClearBackground(RAYWHITE);
        
        ctx.countdownTime -= GetFrameTime();
        if (ctx.countdownTime < 0.0f){
            Text_advance(ctx.text, 1);
            Ctx_calcualte_coundown_time_from_message(&ctx, &ctx.text->feed->messages[ctx.text->feed->cursor]);
        }
        
        TextWindow_render(&ctx, &window);
        EndDrawing();
    }
}