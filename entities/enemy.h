#pragma once

typedef struct Enemy {
    GameObject   base;
    SDL_Texture *spritesheets
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

void enemy_move(Enemy *enemy, float delta_time)
{

}

void enemy_update(GameObject *gameobject, float delta_time, int current_frame)
{

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

void enemy_create_spritesheets(Enemy *enemy)
{

}

void enemy_create(Enemy *enemy, Vector2f position)
{
    enemy->base.type             = ENEMY;
    enemy->base.components       = COLLISION | TEXTURE | ANIMATION;
    enemy->base.position         = vector_create()
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
