#pragma once
#include <SDL2/SDL.h>
#include "gameobject.h"
#include "../components/animation.h"
#include "../components/collision.h"
#include "../components/texture.h"
#include "../utils/vector.h"


typedef struct Player {
    GameObject   base;
    SDL_Texture *spritesheets[ACTIONCOUNT][DIRCOUNT];
    Animation    animation;
    Direction    direction;
    Action       action;
    Collision    collision;
    Vector2f     prev_position;
    int          num_coins;
    int          hurt_frame_count;
    int          health;
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

void player_recoil(Player *player, float delta_time)
{
    // TODO - instead of backwards_velocity, take cross product with enemy velocity to get deflection angle
    Vector2f backwards_velocity = vector_negate(player->base.velocity);
    int recoil_speed_scaling = 4;
    player->base.position.x += backwards_velocity.x * player->base.speed * recoil_speed_scaling * delta_time;
    player->base.position.y += backwards_velocity.y * player->base.speed * recoil_speed_scaling * delta_time;

    player->collision.bounds.x = player->base.position.x;
    player->collision.bounds.y = player->base.position.y;
}

void player_handle_collision(GameObject *gameobject, float delta_time)
{
    // TODO - maybe put collision handler function in collision file,
    //        this function would take in pairs of entities and resolve collisions

    Player *player = (Player *)gameobject;
    for (int i = 0; i < MAX_GAMEOBJECTS; i++) {
        GameObject *obj = gamestate.gameobjects[i];
        if (obj != NULL && obj->type == ENEMY) {
            Enemy *enemy = (Enemy *)obj;
            if (collision_detect(enemy->collision.bounds, player->collision.bounds)) {
                if (player->animation.playing) {
                    animation_stop(&player->animation);
                }
                player->action = HURT;
                animation_start(&player->animation, 4);
                player->health--;
            }
        }
    }
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


    if (player->action == HURT) {
        if (player->hurt_frame_count < 5) {
            player_recoil(player, delta_time);
        } else if (player->hurt_frame_count > 5) {
            if (player->health == 0) {
                // TODO - death animation
                player->base.alive = 0;
                gamestate.mode = MENU; // TODO - gameover screen
            }
            player->action = IDLE;
            player->hurt_frame_count = 0;
            player->base.velocity = vector_create_zero();
        }
        player->hurt_frame_count++;
    } else {
        player_move(player, delta_time);
    }


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

    if (player->action != HURT) {
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
    // TODO - store these elsewhere and only store an index into the array here
    player->spritesheets[0][0] = texture_create("resources/spritesheets/player_idle_front.png");
    player->spritesheets[0][1] = texture_create("resources/spritesheets/player_idle_back.png");
    player->spritesheets[0][2] = texture_create("resources/spritesheets/player_idle_left.png");
    player->spritesheets[0][3] = texture_create("resources/spritesheets/player_idle_right.png");
    player->spritesheets[1][0] = texture_create("resources/spritesheets/player_walking_front.png");
    player->spritesheets[1][1] = texture_create("resources/spritesheets/player_walking_back.png");
    player->spritesheets[1][2] = texture_create("resources/spritesheets/player_walking_left.png");
    player->spritesheets[1][3] = texture_create("resources/spritesheets/player_walking_right.png");
    player->spritesheets[2][0] = texture_create("resources/spritesheets/player_hurt.png");
    player->spritesheets[2][1] = texture_create("resources/spritesheets/player_hurt.png");
    player->spritesheets[2][2] = texture_create("resources/spritesheets/player_hurt.png");
    player->spritesheets[2][3] = texture_create("resources/spritesheets/player_hurt.png");
}

void player_create(Player *player, Vector2f position)
{
    player->base.type             = PLAYER;
    player->base.components       = COLLISION | TEXTURE | ANIMATION | CONTROLLER;
    player->base.position         = position;
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
    player->hurt_frame_count      = 0;
    player->health                = 3;

    player_load_spritesheets(player);
}
