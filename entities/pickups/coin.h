#pragma once
#include <SDL2/SDL.h>
#include "../../gamestate.h"
#include "../../components/animation.h"
#include "../../components/collision.h"
#include "../../entities/gameobject.h"
#include "../../utils/vector.h"

typedef struct Coin {
    GameObject   base;
    SDL_Texture *spritesheet;
    Animation    animation;
    Collision    collision;
} Coin;

void coin_render(GameObject *gameobject)
{
    Coin *coin = (Coin *)gameobject;
    texture_render_clipped(coin->spritesheet,
                           vector_ftoi(coin->base.position),
                           coin->animation.stencil);
}

void coin_render_collision(GameObject *gameobject)
{
    Coin *coin = (Coin *)gameobject;
    collision_render(&coin->collision);
}

void coin_handle_collision(GameObject *gameobject)
{
    //Coin *coin = (Coin *)gameobject;
    //Player *player = (Player *)gamestate.current_scene->gameobjects[0];

    //if (collision_detect(coin->collision.bounds, player->collision.bounds)) {
        // TODO - play sound
        //coin->base.alive = 0;
        //gamestate.ui->hud.num_coins++;
    //}
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
    coin->base.type             = COIN;
    coin->base.components       = COLLISION | TEXTURE | ANIMATION;

    coin->base.position         = vector_create((gamestate.internal_screen_width / 2) + 10, (gamestate.internal_screen_height / 2) + 10);
    coin->base.speed            = 0;
    coin->base.velocity         = vector_create_zero();
    coin->base.alive            = 1;
    coin->base.solid            = 1;
    coin->base.update           = coin_update;
    coin->base.render           = coin_render;
    coin->base.render_collision = coin_render_collision;
    coin->base.handle_collision = coin_handle_collision;
    coin->base.destroy          = coin_destroy;

    coin->animation             = animation_create();
    coin->spritesheet           = texture_create("resources/spritesheets/coin.png");
    coin->collision             = collision_create(coin->base.position, 8, 8);
}
