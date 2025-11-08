#pragma once
#include <SDL2/SDL.h>

#include <stdlib.h>
#include <limits.h>

#include "gameobject.h"
#include "../components/animation.h"
#include "../components/collision.h"
#include "../components/texture.h"
#include "../utils/vector.h"

typedef struct Enemy {
    GameObject   base;
    SDL_Texture *spritesheets[ACTIONCOUNT][DIRCOUNT];
    Animation    animation;
    Collision    collision;
    Action       action;
    Direction    direction;
} Enemy;

void enemy_render(GameObject *gameobject)
{
    Enemy *enemy = (Enemy *)gameobject;
    texture_render_clipped(enemy->spritesheets[enemy->action][enemy->direction],
                           vector_ftoi(enemy->base.position),
                           enemy->animation.stencil);
}

void enemy_render_collision(GameObject *gameobject)
{
    Enemy *enemy = (Enemy *)gameobject;
    collision_render(&enemy->collision);
}

void enemy_handle_collision(GameObject *gameobject, float delta_time)
{
    // handled in player collisions
}

void enemy_move(Enemy *enemy, float delta_time, int current_frame)
{
    if (current_frame % 120 == 0) {
        float random_x = (float)rand();
        if (rand() % 2 == 0) {
            random_x *= -1;
        }
        float random_y = (float)rand();
        if (rand() % 2 == 0) {
            random_y *= -1;
        }
        enemy->base.velocity.x = random_x;
        enemy->base.velocity.y = random_y;

    } else if (current_frame % 20 == 0) {
        enemy->base.velocity = vector_create_zero();
    }
    enemy->base.velocity = vector_normalize(enemy->base.velocity);

    enemy->base.position.x += enemy->base.velocity.x * enemy->base.speed * delta_time;
    enemy->base.position.y += enemy->base.velocity.y * enemy->base.speed * delta_time;
    enemy->collision.bounds.x = enemy->base.position.x;
    enemy->collision.bounds.y = enemy->base.position.y;

}

void enemy_update(GameObject *gameobject, float delta_time, int current_frame)
{
    Enemy *enemy = (Enemy *)gameobject;

    if (enemy->animation.playing) {
        animation_update(&enemy->animation, current_frame);
    }

    if (enemy->action == IDLE) {
        // start blinking every 4 seconds
        if (current_frame % (60*4) == 0) {
            animation_start(&enemy->animation, 4);
        }
    } else if (!enemy->animation.playing) {
        animation_start(&enemy->animation, 4);
    }

    enemy_move(enemy, delta_time, current_frame);
}

void enemy_destroy(GameObject *gameobject)
{
    Enemy *enemy = (Enemy *)gameobject;

    for (int i = 0; i < ACTIONCOUNT; i++) {
        for (int j = 0; j < DIRCOUNT; j++) {
            SDL_DestroyTexture(enemy->spritesheets[i][j]);
        }
    }
}

void enemy_load_spritesheets(Enemy *enemy)
{
    enemy->spritesheets[0][0] = texture_create("resources/spritesheets/enemy_idle_front.png");
    enemy->spritesheets[0][1] = texture_create("resources/spritesheets/enemy_idle_back.png");
    enemy->spritesheets[0][2] = texture_create("resources/spritesheets/enemy_idle_left.png");
    enemy->spritesheets[0][3] = texture_create("resources/spritesheets/enemy_idle_right.png");
    //enemy->spritesheets[1][0] = texture_create("resources/spritesheets/enemy_walking_front.png");
    //enemy->spritesheets[1][1] = texture_create("resources/spritesheets/enemy_walking_back.png");
    //enemy->spritesheets[1][2] = texture_create("resources/spritesheets/enemy_walking_left.png");
    //enemy->spritesheets[1][3] = texture_create("resources/spritesheets/enemy_walking_right.png");
}

void enemy_create(Enemy *enemy, Vector2f position)
{
    enemy->base.type             = ENEMY;
    enemy->base.components       = COLLISION | TEXTURE | ANIMATION;
    enemy->base.position         = position;
    enemy->base.velocity         = vector_create_zero();
    enemy->base.speed            = 100.f;
    enemy->base.alive            = 1;
    enemy->base.solid            = 1;

    enemy->base.update           = enemy_update;
    enemy->base.render           = enemy_render;
    enemy->base.render_collision = enemy_render_collision;
    enemy->base.handle_collision = enemy_handle_collision;
    enemy->base.destroy          = enemy_destroy;

    enemy->animation             = animation_create();
    enemy->direction             = DOWN;
    enemy->action                = IDLE;
    enemy->collision             = collision_create(enemy->base.position, 8, 8);

    enemy_load_spritesheets(enemy);
}
