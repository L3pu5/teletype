#ifndef _BINDINGS
#define _BINDINGS
#include <raylib.h>
#include "config.h"
#include "message.h"

typedef struct _TextWindow {
    int offsetX;
    int offsetY;
    int height;
    int width;
} TextWindow;

void TextWindow_render(ctx* ctx, TextWindow* window);
void TextWindow_transform_message(ctx* ctx, TextWindow* window, Msg* msg);

#endif