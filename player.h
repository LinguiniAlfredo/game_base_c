#pragma once

typedef struct Player {
  int alive;
  float x, y;
  float speed;
  float vel_x, vel_y;
  SDL_Texture *texture;

} Player; // 28 bytes


void player_render(Player *player)
{
    texture_render(gamestate.renderer, player->texture, (int)player->x,
                 (int)player->y);
}

void player_move(Player *player, float delta_time)
{
  player->x += player->vel_x * player->speed * delta_time;
  player->y += player->vel_y * player->speed * delta_time;
}

void player_update(Player *player, float delta_time)
{
  player_move(player, delta_time);
}

void player_update_texture(Player *player, char* filepath)
{
    if (player->texture != NULL) {
        SDL_DestroyTexture(player->texture);
    }
    player->texture = texture_create(gamestate.renderer, filepath);
}

void player_handle_events(Player *player, SDL_Event *e)
{
    if (e->type == SDL_KEYDOWN && e->key.repeat == 0) {
        SDL_Keycode key = e->key.keysym.sym;
        if (key == SDLK_a) {
            player->vel_x = -1.f;
            player_update_texture(player, "resources/textures/player_left.png");
        }
        if (key == SDLK_d) {
            player->vel_x = 1.f;
            player_update_texture(player, "resources/textures/player_right.png");
        }
        if (key == SDLK_w) {
            player->vel_y = -1.f;
            player_update_texture(player, "resources/textures/player_back.png");
        }
        if (key == SDLK_s) {
            player->vel_y = 1.f;
            player_update_texture(player, "resources/textures/player_front.png");
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
  printf("destroying texture...\n");
  SDL_DestroyTexture(player->texture);
}

void player_create(Player *player)
{
  printf("creating player...\n");
  player->x = 0;
  player->y = 0;
  player->speed = 100.f;
  player->vel_x = 0;
  player->vel_y = 0;
  player->alive = 1;
  player->texture =
      texture_create(gamestate.renderer, "resources/textures/player_front.png");
}
