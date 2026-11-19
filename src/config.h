#ifndef _CONFIG
#define _CONFIG
#include <raylib.h>
#include "message.h"

#define CTX_DEFAULT_FontSize 10
#define CTX_DEFAULT_Font GetFontDefault()
#define CTX_DEFAULT_PaddingY 2

typedef struct Context {
    int words_per_minute;
    bool transparent_window;
    Text* text;

    int height;
    int width;
    int fontSize;
    Font font;
    int paddingY;

    float totalTime;
    float countdownTime;
} ctx;

void Ctx_init_default(ctx* ctx);
void Ctx_calcualte_coundown_time_from_message(ctx* ctx, Msg* msg);

#endif