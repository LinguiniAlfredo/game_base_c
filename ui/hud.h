#pragma once

typedef struct Hud {
    TTF_Font    *font;
    SDL_Texture *texture;
    SDL_Color    color;
    Vector2f     position;
} Hud;

void hud_render(Hud *hud)
{
}

void hud_update(Hud *hud, float fps)
{
}

void hud_create(Hud *hud)
{
    hud->position    = vector_create_zero();
    hud->color.r     = 0;
    hud->color.g     = 0;
    hud->color.b     = 0;
    hud->color.a     = 255;

    hud->font        = TTF_OpenFont("resources/fonts/nes.ttf", 10);
    hud->texture     = texture_create_text("", hud->font, hud->color, 5, 5);
}

void hud_destroy(Hud *hud)
{
    TTF_CloseFont(hud->font);
    SDL_DestroyTexture(hud->texture);
}
