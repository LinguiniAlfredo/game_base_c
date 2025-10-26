#pragma once

typedef enum Direction {
    FRONT,
    BACK,
    LEFT,
    RIGHT,
    DIRCOUNT
} Direction;

typedef enum Action {
    IDLE,
    WALKING,
    ACTIONCOUNT
} Action;

typedef struct Player {
    int alive;
    float x, y;
    float speed;
    float vel_x, vel_y;
    SDL_Texture *spritesheets[ACTIONCOUNT][DIRCOUNT];
    Animation animation;
    Direction direction;
    Action action;

} Player;


void player_render(Player *player)
{
    texture_render_clipped(player->spritesheets[player->action][player->direction], (int)player->x,
                 (int)player->y, player->animation.stencil);
}

void player_move(Player *player, float delta_time)
{
    player->x += player->vel_x * player->speed * delta_time;
    player->y += player->vel_y * player->speed * delta_time;
}

void player_update(Player *player, float delta_time, int current_frame)
{
    player_move(player, delta_time);

    if (player->action == IDLE) {
        if (current_frame % (60*4) == 0) {
            animation_start(&player->animation);
        }
    } else if (!player->animation.playing) {
        animation_start(&player->animation);
    }

    if (player->animation.playing) {
        animation_update(&player->animation, current_frame);
    }
}

void player_handle_events(Player *player, SDL_Event *e)
{
    if (e->type == SDL_KEYDOWN && e->key.repeat == 0) {
        SDL_Keycode key = e->key.keysym.sym;
        if (key == SDLK_a) {
            player->direction = LEFT;
            player->action = WALKING;
            player->vel_x = -1.f;
        }
        if (key == SDLK_d) {
            player->direction = RIGHT;
            player->action = WALKING;
            player->vel_x = 1.f;
        }
        if (key == SDLK_w) {
            player->direction = BACK;
            player->action = WALKING;
            player->vel_y = -1.f;
        }
        if (key == SDLK_s) {
            player->direction = FRONT;
            player->action = WALKING;
            player->vel_y = 1.f;
        }
    }
    if (e->type == SDL_KEYUP && e->key.repeat == 0) {
        SDL_Keycode key = e->key.keysym.sym;
        if (key == SDLK_a) {
            player->action = IDLE;
            if (player->vel_x < 0) {
                player->vel_x = 0;
            }
        }
        if (key == SDLK_d) {
            player->action = IDLE;
            if (player->vel_x > 0) {
                player->vel_x = 0;
            }
        }
        if (key == SDLK_w) {
            player->action = IDLE;
            player->vel_y = 0;
        }
        if (key == SDLK_s) {
            player->action = IDLE;
            player->vel_y = 0;
        }
    }
}

void player_destroy(Player *player)
{
    for (int i = 0; i < ACTIONCOUNT; i++) {
        for (int j = 0; j < DIRCOUNT; j++) {
            SDL_DestroyTexture(player->spritesheets[i][j]);
        }
    }
    animation_destroy(&player->animation);
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
}

void player_create(Player *player)
{
    player->x = gamestate.internal_screen_width / 2;
    player->y = gamestate.internal_screen_height / 2;
    player->speed = 100.f;
    player->vel_x = 0;
    player->vel_y = 0;
    player->alive = 1;
    player->animation = animation_create(); // TODO parameterize animation properties
    player->direction = FRONT;
    player->action = IDLE;
    player_load_spritesheets(player);

}
