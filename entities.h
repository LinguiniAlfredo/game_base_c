#pragma once


typedef struct Entity {
    int x, y;
    int vel_x, vel_y;

    int alive;

    SDL_Texture* texture;

    void (*draw)(struct Entity*, SDL_Renderer* renderer);
    void (*handle_events)(struct Entity*, SDL_Event* e);
    void (*update)(struct Entity*);
    void (*destroy)(struct Entity*);
} Entity;

typedef struct {
    Entity base;
} Player;

void player_draw(Entity* entity, SDL_Renderer* renderer)
{
    Player* player = (Player*)entity;
    texture_render(renderer, player->base.texture, player->base.x, player->base.y);
}

void player_move(Player* player) 
{
    player->base.x += player->base.vel_x * gamestate.tile_size;
    player->base.y += player->base.vel_y * gamestate.tile_size;
	player->base.vel_x = 0;
	player->base.vel_y = 0;
}

void player_update(Entity* entity)
{
    Player* player = (Player*)entity;
    player_move(player);
}

void player_handle_events(Entity* entity, SDL_Event* e) 
{
    Player* player = (Player*)entity;
    if (e->type == SDL_KEYDOWN && e->key.repeat == 0) {
		SDL_Keycode key = e->key.keysym.sym;
		if (key == SDLK_a) {
			player->base.vel_x = -1;
		}
		if (key == SDLK_d) {
			player->base.vel_x = 1;
		}
		if (key == SDLK_w) {
			player->base.vel_y = -1;
		}
		if (key == SDLK_s) {
			player->base.vel_y = 1;
		}
	}
	if (e->type == SDL_KEYUP && e->key.repeat == 0) {
		SDL_Keycode key = e->key.keysym.sym;
		if (key == SDLK_a) {
			if (player->base.vel_x < 0) {
				player->base.vel_x = 0;
			}
		} if (key == SDLK_d) {
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

void player_destroy(Entity* entity) 
{
    printf("destroying texture...\n");    
    Player* player = (Player*)entity;
    SDL_DestroyTexture(player->base.texture);
}

void player_create(Player* player, SDL_Renderer* renderer)
{
    printf("creating player...\n");
    player->base.x = 0;
    player->base.y = 0;
    player->base.vel_x = 0;
    player->base.vel_y = 0;

    player->base.alive = 1;

    player->base.draw = player_draw;
    player->base.update = player_update;
    player->base.handle_events = player_handle_events;
    player->base.destroy = player_destroy;

    player->base.texture = texture_create(renderer, "resources/textures/player.png");
}

