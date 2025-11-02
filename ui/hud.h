#pragma once

typedef struct Hud {
    TTF_Font    *font;
    SDL_Texture *fps_texture;
    SDL_Color    color;
    Vector2f     position;
} Hud;

void hud_render(Hud *hud)
{
    texture_render(hud->fps_texture, vector_ftoi(hud->position));
}

void hud_update(Hud *hud, float fps)
{
    if (hud->fps_texture != NULL) {
        SDL_DestroyTexture(hud->fps_texture);
    }
    char fps_label[] = "FPS: ";
    char fps_value[50];
    sprintf(fps_value, "%.2f", fps);
    strcat(fps_label, fps_value);
    hud->fps_texture = texture_create_text(fps_label, hud->font, hud->color, 5, 5);
}

void hud_create(Hud *hud)
{
    hud->position    = vector_create_zero();
    hud->color.r     = 0;
    hud->color.g     = 0;
    hud->color.b     = 0;
    hud->color.a     = 255;

    hud->font        = TTF_OpenFont("resources/fonts/nes.ttf", 10);
    hud->fps_texture = texture_create_text("FPS: 0", hud->font, hud->color, 5, 5);
}

void hud_destroy(Hud *hud)
{
    TTF_CloseFont(hud->font);
    SDL_DestroyTexture(hud->fps_texture);
}
