#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "../components/texture.h"
#include "../utils/vector.h"
#include "../gamestate.h"
#include "../entities/player.h"

typedef struct Hud {
    TTF_Font    *font;
    SDL_Texture *text_texture;
    SDL_Texture *icon_texture;
    SDL_Color    color;
    Vector2f     position;
    Animation    animation;
} Hud;

void hud_render(Hud *hud)
{
    texture_render_clipped(hud->icon_texture,
                           vector_ftoi(vector_create(hud->position.x - 10, hud->position.y + 1)),
                           hud->animation.stencil);
    texture_render(hud->text_texture, vector_ftoi(hud->position));
}

void hud_update(Hud *hud, int current_frame)
{
    Player *player = (Player *)gamestate.gameobjects[0];
    
    if (hud->text_texture != NULL) {
        SDL_DestroyTexture(hud->text_texture);
    }
    char coins_label[] = "x";
    char coins_value[50];
    sprintf(coins_value, "%d", player->num_coins);
    strcat(coins_label, coins_value);
    hud->text_texture = texture_create_text(coins_label, hud->font, hud->color, 5, 5);

    if (hud->animation.playing) {
        animation_update(&hud->animation, current_frame);
    } else {
        animation_start(&hud->animation, 8);
    }
}

void hud_create(Hud *hud)
{
    hud->color.r      = 0;
    hud->color.g      = 0;
    hud->color.b      = 0;
    hud->color.a      = 255;

    hud->font         = TTF_OpenFont("resources/fonts/nes.ttf", 10);
    hud->icon_texture = texture_create("resources/spritesheets/coin.png");
    hud->text_texture = texture_create_text("x0", hud->font, hud->color, 5, 5);
    hud->position     = vector_create(20, 5);
    hud->animation    = animation_create();
}

void hud_destroy(Hud *hud)
{
    TTF_CloseFont(hud->font);
    SDL_DestroyTexture(hud->icon_texture);
    SDL_DestroyTexture(hud->text_texture);
}
