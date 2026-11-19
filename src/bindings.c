#include <stdlib.h>
#include <string.h>
#include "bindings.h"
#include <raylib.h>
#include <math.h>


int max(int y, int x){
    if (x < y)
        return y;
    return x;
}

int nlines(int span, int window) {
    int lines = 0;
    while(span > 0) {
        lines++;
        span -= window;
    }
    return lines;
}

void TextWindow_transform_message(ctx* ctx, TextWindow* window, Msg* msg) {
    if (msg->has.transformed == true)
        return;
        
    char tempBuffer[1000] = {0};
    memcpy(tempBuffer, msg->pText, msg->szMessage);

    int szText = MeasureText(tempBuffer, ctx->fontSize);

    // 1 Line?
    if ( szText < window->width) {
        // No multiline, set lines and render.
        msg->has.lines = 1;
        return;
    }

    char lineBuffer[1000] = {0};
    int previousWidth = 0;
    int nLines = nlines(szText, window->width);
    int totalLines = nLines;
    
    char* previousC = tempBuffer; 
    char* previousW = tempBuffer;
    char* c = previousC;
    do {
        while (c < tempBuffer + msg->szMessage && nLines > 1) {
            if (*c == ' ') {
                // Measure from previousC to this width.
                memcpy(lineBuffer, previousC, (c-previousC));
                int w = MeasureText(lineBuffer, ctx->fontSize);
                if (w < window->width) {
                    // We are not long enough, continue;
                    previousW = c;
                    c++; 
                    continue;
                }

                // This is long enough to chop, make it a new line 
                // However, we want to use the previous word.
                *previousW = '\n';
                previousC = previousW;
                c = previousC;
                nLines--;
                // Eliminate lineBuffer
                memset(lineBuffer, 0, 1000);
            }
            c++;
        } 
    } while (nLines > 1);

    // Update the raw source
    memcpy(msg->pText, tempBuffer, msg->szMessage);
    msg->has.lines = totalLines;
    return;
}

void TextWindow_transform_feed(ctx* ctx, TextWindow* window) {
    Feed* feed = ctx->text->feed;
    for (size_t i = 0; i < feed->nMessages; i++)
    {
        TextWindow_transform_message(ctx, window, &feed->messages[i]);
    }
}

void TextWindow_render(ctx* ctx, TextWindow* window){
    Msg* renderText = ctx->text->feed->messages;
    int cursor = ctx->text->feed->cursor;    
    
    // // Render the previous up to the cursor.
    // // From the start up to the cursor
    // // Copy to a new buffer to avoid overflowing
    char tempBuffer[1000];
    // For the rendered window we want to start with the lines and go back to the index that is most visible 
    
    // Start at Cursor and count backwards.
    int messageIndex = cursor;
    int remainingY = window->height;
    int totalMessageCount = 0;

    while (remainingY > 0) {
        memset(tempBuffer, 0, 1000);
        TextWindow_transform_message(ctx, window, &renderText[messageIndex]);
        memcpy(tempBuffer, renderText[cursor].pText, renderText[cursor].szMessage); 
        remainingY -= (int)MeasureTextEx(GetFontDefault(), tempBuffer, ctx->fontSize, 1.0f).y;
        messageIndex--;
        totalMessageCount++;
    }
    
    // Restrain messageIndex = 0;
    messageIndex = max(messageIndex, 0);
    int offsetY= 0;

    // Single Cursor
    if(cursor == messageIndex) {
        memset(tempBuffer, 0, 1000);
        TextWindow_transform_message(ctx, window, &renderText[cursor]);
        memcpy(tempBuffer, renderText[cursor].pText, renderText[cursor].szMessage); 
        Vector2 bounds = MeasureTextEx(GetFontDefault(), tempBuffer, ctx->fontSize, 1.0f);
        offsetY += (int)bounds.y;
        DrawText(tempBuffer, window->offsetX, (window->offsetY+window->height-offsetY), ctx->fontSize, BLACK);
        return;
    }

    int messageCount = 0;
    for (size_t i = cursor; i >= messageIndex && i >= 0; i--)
    {
        if( i == -1)
            break;

        // Copy to the buffer;
        if (renderText[i].szMessage > 1000) {
            printf("Text too large, TODO: implement dynamic buffer\n");
            printf("Got buffer with sz: %d, '%s'\n", renderText[i].szMessage, renderText[i].pText);
            Msg_print(&renderText[i]);
            exit(4);
        }
        
        int alpha = 255;
        if (messageCount != 0){
            float pct = ((float)messageCount/(float)totalMessageCount);
            if (pct > 1.0f) {
                pct = 0.0f;
            }
            float transparency = 0.6f * (1.0f - pct);
            alpha = (int)(transparency*alpha);
        } 

        Color c = (Color) {0, 0, 0, alpha};

        memset(tempBuffer, 0, 1000);
        // Transform the message if required
        TextWindow_transform_message(ctx, window, &renderText[i]);
        // Copy it to the working buffer
        memcpy(tempBuffer, renderText[i].pText, renderText[i].szMessage); 
        Vector2 bounds = MeasureTextEx(GetFontDefault(), tempBuffer, ctx->fontSize, 1.0f);
        
        // Render the text
        offsetY += (int)bounds.y;
        DrawText(tempBuffer, window->offsetX, (window->offsetY+window->height- offsetY), ctx->fontSize, c);
        messageCount++;
    };

}
