#pragma once
#include <SDL2/SDL.h>
#include "gameobject.h"
#include "../components/animation.h"
#include "../components/collision.h"
#include "../components/texture.h"
#include "../utils/vector.h"

typedef enum {
    DOWN,
    UP,
    LEFT,
    RIGHT,
    DIRCOUNT
} Direction;

typedef enum {
    IDLE,
    WALKING,
    SLEEPING,
    ACTIONCOUNT
} Action;

typedef struct Player {
    GameObject   base;
    SDL_Texture *spritesheets[ACTIONCOUNT][DIRCOUNT];
    Animation    animation;
    Direction    direction;
    Action       action;
    Collision    collision;
    Vector2f     prev_position;
    int          num_coins;
} Player;


void player_render(GameObject *gameobject)
{
    Player *player = (Player *)gameobject;

    texture_render_clipped(player->spritesheets[player->action][player->direction],
                           vector_ftoi(player->base.position),
                           player->animation.stencil);
}

void player_render_collision(GameObject *gameobject) {
    Player *player = (Player *)gameobject;
    collision_render(&player->collision);
}

void player_handle_collision(GameObject *gameobject)
{
    //Player *player = (Player *)gameobject;
    //for (int i = 0; i < MAX_GAMEOBJECTS; i++) {
    //    GameObject *obj = gameobjects[i];
    //    if (obj != NULL && obj->type != PLAYER && obj->solid && obj->components & COLLISION) {
    //        switch (obj->type) {
    //            case COIN: {
    //                Coin *coin = (Coin *)obj;
    //                if (collision_detect(player->collision.bounds, coin->collision.bounds)) {
    //                    // TODO - play sound, increment coin count on hud
    //                    coin->alive = 0;
    //                }
    //            } break;
    //            case PLAYER:
    //                break;
    //            case BLOCK:
    //                break;
    //        }
    //    }
    //}
}

void player_move(Player *player, float delta_time)
{
    // store away previous position for collision to revert if necessary
    player->prev_position.x = player->base.position.x;
    player->prev_position.y = player->base.position.y;

    player->base.velocity = vector_normalize(player->base.velocity);

    player->base.position.x += player->base.velocity.x * player->base.speed * delta_time;
    player->base.position.y += player->base.velocity.y * player->base.speed * delta_time;

    player->collision.bounds.x = player->base.position.x;
    player->collision.bounds.y = player->base.position.y;
}

void player_update(GameObject *gameobject, float delta_time, int current_frame)
{
    Player *player = (Player *)gameobject;

    player_move(player, delta_time);

    if (player->animation.playing) {
        animation_update(&player->animation, current_frame);
    }

    if (player->action == IDLE) {
        // start blinking every 4 seconds
        if (current_frame % (60*4) == 0) {
            animation_start(&player->animation, 4);
        }
    } else if (!player->animation.playing) {
        animation_start(&player->animation, 4);
    }
}

void player_handle_events(GameObject *gameobject, SDL_Event *e)
{
    Player *player = (Player *)gameobject;

    if (e->type == SDL_KEYDOWN && e->key.repeat == 0) {
        SDL_Keycode key = e->key.keysym.sym;
        if (key == SDLK_a) {
            player->direction = LEFT;
            player->base.velocity.x = -1.f;
        }
        if (key == SDLK_d) {
            player->direction = RIGHT;
            player->base.velocity.x = 1.f;
        }
        if (key == SDLK_w) {
            player->direction = UP;
            player->base.velocity.y = -1.f;
        }
        if (key == SDLK_s) {
            player->direction = DOWN;
            player->base.velocity.y = 1.f;
        }
    }
    if (e->type == SDL_KEYUP && e->key.repeat == 0) {
        SDL_Keycode key = e->key.keysym.sym;
        if (key == SDLK_a || key == SDLK_d) {
            player->base.velocity.x = 0;
        }
        if (key == SDLK_w || key == SDLK_s) {
            player->base.velocity.y = 0;
        }
    }

    if (player->base.velocity.x == 0 && player->base.velocity.y == 0) {
        player->action = IDLE;
    } else {
        player->action = WALKING;
    }
}

void player_destroy(GameObject *gameobject)
{
    Player *player = (Player *)gameobject;

    for (int i = 0; i < ACTIONCOUNT; i++) {
        for (int j = 0; j < DIRCOUNT; j++) {
            SDL_DestroyTexture(player->spritesheets[i][j]);
        }
    }
}

void player_load_spritesheets(Player *player)
{
    // TODO - figure out how to do this in a loop
    // for (int i = 0; i < ACTIONCOUNT; i++)
    //      for (int j = 0; j < DIRCOUNT; j++)
    //          etc...
    
    player->spritesheets[0][0] = texture_create("resources/spritesheets/player_idle_front.png");
    player->spritesheets[0][1] = texture_create("resources/spritesheets/player_idle_back.png");
    player->spritesheets[0][2] = texture_create("resources/spritesheets/player_idle_left.png");
    player->spritesheets[0][3] = texture_create("resources/spritesheets/player_idle_right.png");
    player->spritesheets[1][0] = texture_create("resources/spritesheets/player_walking_front.png");
    player->spritesheets[1][1] = texture_create("resources/spritesheets/player_walking_back.png");
    player->spritesheets[1][2] = texture_create("resources/spritesheets/player_walking_left.png");
    player->spritesheets[1][3] = texture_create("resources/spritesheets/player_walking_right.png");
    player->spritesheets[2][0] = texture_create("resources/spritesheets/player_sleeping_front.png");
}

void player_create(Player *player)
{
    player->base.type             = PLAYER;
    player->base.components       = COLLISION | TEXTURE | ANIMATION | CONTROLLER;

    player->base.position         = vector_create(gamestate.internal_screen_width / 2, gamestate.internal_screen_height / 2);
    player->base.velocity         = vector_create_zero();
    player->base.speed            = 100.f;
    player->base.alive            = 1;
    player->base.solid            = 1;
    player->base.update           = player_update;
    player->base.render           = player_render;
    player->base.render_collision = player_render_collision;
    player->base.handle_collision = player_handle_collision;
    player->base.handle_events    = player_handle_events;
    player->base.destroy          = player_destroy;

    player->prev_position         = vector_create_zero();

    player->animation             = animation_create();
    player->direction             = DOWN;
    player->action                = IDLE;
    player->collision             = collision_create(player->base.position, 8, 8);

    player->num_coins             = 0;

    player_load_spritesheets(player);
}
