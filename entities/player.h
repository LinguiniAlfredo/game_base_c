#pragma once

// TODO - store all spritesheets, based on direction / action enum, index to proper spritesheet, then animations should just work for each direction / action

typedef struct Player {
    int alive;
    float x, y;
    float speed;
    float vel_x, vel_y;
    SDL_Texture *spritesheet;
    Animation animation;

} Player; // 28 bytes


void player_render(Player *player)
{
    texture_render_clipped(gamestate.renderer, player->spritesheet, (int)player->x,
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
    if (current_frame % (60*4) == 0) {
        animation_start(&player->animation);
    }
    if (player->animation.playing) {
        animation_update(&player->animation, current_frame);
    }
}

void player_update_texture(Player *player, char* filepath)
{
    // TODO - update to change animation
    if (player->spritesheet != NULL) {
        SDL_DestroyTexture(player->spritesheet);
    }
    player->spritesheet = texture_create(gamestate.renderer, filepath);
}

void player_handle_events(Player *player, SDL_Event *e)
{
    if (e->type == SDL_KEYDOWN && e->key.repeat == 0) {
        SDL_Keycode key = e->key.keysym.sym;
        if (key == SDLK_a) {
            player->vel_x = -1.f;
            //player_update_texture(player, "resources/textures/player_left.png");
        }
        if (key == SDLK_d) {
            player->vel_x = 1.f;
            //player_update_texture(player, "resources/textures/player_right.png");
        }
        if (key == SDLK_w) {
            player->vel_y = -1.f;
            //player_update_texture(player, "resources/textures/player_back.png");
        }
        if (key == SDLK_s) {
            player->vel_y = 1.f;
            //player_update_texture(player, "resources/textures/player_front.png");
        }
    }
    if (e->type == SDL_KEYUP && e->key.repeat == 0) {
        SDL_Keycode key = e->key.keysym.sym;
        if (key == SDLK_a) {
            if (player->vel_x < 0) {
                player->vel_x = 0;
            }
        }
        if (key == SDLK_d) {
            if (player->vel_x > 0) {
                player->vel_x = 0;
            }
        }
        if (key == SDLK_w) {
            player->vel_y = 0;
        }
        if (key == SDLK_s) {
            player->vel_y = 0;
        }
    }
}

void player_destroy(Player *player)
{
    SDL_DestroyTexture(player->spritesheet);
    animation_destroy(&player->animation);
}

void player_create(Player *player)
{
    player->x = 0;
    player->y = 0;
    player->speed = 100.f;
    player->vel_x = 0;
    player->vel_y = 0;
    player->alive = 1;
    player->spritesheet =
        texture_create(gamestate.renderer, "resources/spritesheets/player_front_idle.png");
    player->animation = animation_create();
}
