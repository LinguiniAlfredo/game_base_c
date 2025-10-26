#pragma once

typedef struct Entity {
  int alive;
  float x, y;
  float speed;
  float vel_x, vel_y;

  SDL_Texture *texture;

  void (*render)(struct Entity *);
  void (*handle_events)(struct Entity *, SDL_Event *e);
  void (*update)(struct Entity *, float delta_time);
  void (*destroy)(struct Entity *);
} Entity;


typedef struct {
  Entity base;
} Player;

typedef struct {
  Entity base;
} Enemy;


void player_render(Entity *entity)
{
    Player *player = (Player *)entity;
    texture_render(gamestate.renderer, player->base.texture, (int)player->base.x,
                 (int)player->base.y);
}

void player_move(Player *player, float delta_time)
{
  player->base.x += player->base.vel_x * player->base.speed * delta_time;
  player->base.y += player->base.vel_y * player->base.speed * delta_time;
}

void player_update(Entity *entity, float delta_time)
{
  Player *player = (Player *)entity;
  player_move(player, delta_time);
}

void player_update_texture(Player *player, char* filepath)
{
    if (player->base.texture != NULL) {
        SDL_DestroyTexture(player->base.texture);
    }
    player->base.texture = texture_create(gamestate.renderer, filepath);
}

void player_handle_events(Entity *entity, SDL_Event *e)
{
    Player *player = (Player *)entity;

    if (e->type == SDL_KEYDOWN && e->key.repeat == 0) {
        SDL_Keycode key = e->key.keysym.sym;
        if (key == SDLK_a) {
            player->base.vel_x = -1.f;
            player_update_texture(player, "resources/textures/player_left.png");
        }
        if (key == SDLK_d) {
            player->base.vel_x = 1.f;
            player_update_texture(player, "resources/textures/player_right.png");
        }
        if (key == SDLK_w) {
            player->base.vel_y = -1.f;
            player_update_texture(player, "resources/textures/player_back.png");
        }
        if (key == SDLK_s) {
            player->base.vel_y = 1.f;
            player_update_texture(player, "resources/textures/player_front.png");
        }
    }
    if (e->type == SDL_KEYUP && e->key.repeat == 0) {
        SDL_Keycode key = e->key.keysym.sym;
        if (key == SDLK_a) {
            if (player->base.vel_x < 0) {
                player->base.vel_x = 0;
            }
        }
        if (key == SDLK_d) {
            if (player->base.vel_x > 0) {
                player->base.vel_x = 0;
            }
        }
        if (key == SDLK_w) {
            player->base.vel_y = 0;
        }
        if (key == SDLK_s) {
            player->base.vel_y = 0;
        }
    }
}

void player_destroy(Entity *entity)
{
  printf("destroying texture...\n");
  Player *player = (Player *)entity;
  SDL_DestroyTexture(player->base.texture);
}

void player_create(Player *player)
{
  printf("creating player...\n");
  player->base.x = 0;
  player->base.y = 0;
  player->base.speed = 100.f;
  player->base.vel_x = 0;
  player->base.vel_y = 0;


  player->base.alive = 1;

  player->base.render = player_render;
  player->base.update = player_update;
  player->base.handle_events = player_handle_events;
  player->base.destroy = player_destroy;

  player->base.texture =
      texture_create(gamestate.renderer, "resources/textures/player_front.png");
}
