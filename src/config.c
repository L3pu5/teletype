#include "config.h"

void Ctx_init_default(ctx* ctx){
    ctx->paddingY = CTX_DEFAULT_PaddingY;
    ctx->fontSize = CTX_DEFAULT_FontSize;
    ctx->font = GetFontDefault();
    ctx->words_per_minute = 200;
    ctx->countdownTime = 0.0f;
}

void Ctx_calcualte_coundown_time_from_message(ctx* ctx, Msg* msg){
    float pctWpm = (float)msg->nWords / ctx->words_per_minute;
    ctx->countdownTime = pctWpm * 60.0f;
}