#pragma once

typedef struct Coin {
    GameObject base;

    SDL_Texture *spritesheet;
    Animation animation;
    Collision collision;

} Coin;

void coin_render(GameObject *gameobject)
{
    Coin *coin = (Coin *)gameobject;
    texture_render_clipped(coin->spritesheet, (int)coin->base.pos_x, (int)coin->base.pos_y, coin->animation.stencil);
}

void coin_render_collision(GameObject *gameobject)
{
    Coin *coin = (Coin *)gameobject;
    collision_render(&coin->collision);
}

void coin_update(GameObject *gameobject, float delta_time, int current_frame)
{
    Coin *coin = (Coin *)gameobject;

    if (coin->animation.playing) {
        animation_update(&coin->animation, current_frame);
    } else {
        animation_start(&coin->animation, 8);
    }
}

void coin_destroy(GameObject *gameobject)
{
    Coin *coin = (Coin *)gameobject;
    SDL_DestroyTexture(coin->spritesheet);
}

void coin_create(Coin *coin)
{
    coin->base.components       = COLLISION | TEXTURE | ANIMATION;

    coin->base.pos_x            = (gamestate.internal_screen_width / 2) + 10;
    coin->base.pos_y            = (gamestate.internal_screen_height / 2) + 10;
    coin->base.speed            = 0;
    coin->base.vel_x            = 0;
    coin->base.vel_y            = 0;
    coin->base.alive            = 1;
    coin->base.update           = coin_update;
    coin->base.render           = coin_render;
    coin->base.render_collision = coin_render_collision;
    coin->base.destroy          = coin_destroy;

    coin->animation             = animation_create();
    coin->spritesheet           = texture_create("resources/spritesheets/coin.png");
    coin->collision             = collision_create(coin->base.pos_x, coin->base.pos_y, 8, 8);
}
