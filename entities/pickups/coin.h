#pragma once

typedef struct Coin {
    float pos_x, pos_y;
    Animation animation;
    SDL_Texture *spritesheet;
    int alive;

} Coin;

void coin_render(Coin *coin)
{
    texture_render_clipped(coin->spritesheet, (int)coin->pos_x, (int)coin->pos_y, coin->animation.stencil);
}

void coin_update(Coin *coin, int current_frame)
{
    if (coin->animation.playing) {
        animation_update(&coin->animation, current_frame);
    } else {
        animation_start(&coin->animation, 8);
    }
}

void coin_destroy(Coin *coin)
{
    SDL_DestroyTexture(coin->spritesheet);
}

void coin_create(Coin *coin)
{
    coin->pos_x = (gamestate.internal_screen_width / 2) + 10;
    coin->pos_y = (gamestate.internal_screen_height / 2) + 10;
    coin->animation = animation_create();
    coin->spritesheet = texture_create("resources/spritesheets/coin.png");
    coin->alive = 1;
}
