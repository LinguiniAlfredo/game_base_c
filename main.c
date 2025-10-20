#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include <stdint.h>

#include "gamestate.h"
#include "components/texture.h"
#include "utils/arena.h"
#include "utils/timer.h"
#include "entities.h"

Arena arena;
Entity* entities[1000];

SDL_Renderer *renderer = NULL;
SDL_Window *window = NULL;

int initialize() 
{
	int success = 1;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
	  printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
	} else {
		printf("SDL initialized successfully\n");
		window = SDL_CreateWindow("", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, gamestate.screen_width, gamestate.screen_height, SDL_WINDOW_SHOWN);
		if (window == NULL) {
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			success = 0;
		} else {
            printf("window created successfully\n");
			renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
			if (renderer == NULL) {
				printf("renderer could not be created : %s", SDL_GetError());

			} else {
                printf("renderer created successfully\n");
				SDL_RenderSetLogicalSize(renderer, gamestate.internal_screen_width, gamestate.internal_screen_height);

				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags)) {
					printf("SDL_Image could not init : %s", IMG_GetError());
					success = 0;
				} else {
            printf("SDL_Image initialized successfully\n");
            if (TTF_Init() == -1) {
                printf("SDL_TTF failed to initialize: %s\n", TTF_GetError());
            } else {
                printf("SDL_TTF initialized successfully\n");
            }
				}
			}
		}
	}
	return success;
}

void toggle_debug() 
{
    gamestate.debug = !gamestate.debug;
}

int handle_events() 
{
    SDL_Event e;
    int quit = 0;
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            quit = 1;
        }

        if (e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym) {
                case SDLK_ESCAPE:
                    quit = 1;
                    break;
                case SDLK_1:
                    break;
                case SDLK_F1:
                    toggle_debug();
                    break;
                default:
                    break;
            }
        }

        for (int i = 0; i < 1; i++) {
            if (entities[i]) {
                entities[i]->handle_events(entities[i], &e);
            }
        }
    }
    return quit;
}

void update() 
{
    for (int i = 0; i < 1; i++) {
        if (entities[i]) {
            entities[i]->update(entities[i]);
        }
    }
}

void render() 
{
    SDL_SetRenderDrawColor(renderer, 0xF5, 0xF5, 0xF5, 0xFF);
    SDL_RenderClear(renderer);

    for (int i = 0; i < 1; i++) {
        if (entities[i]) {
            entities[i]->draw(entities[i], renderer);
        }
    }

    SDL_RenderPresent(renderer);
}

void game_loop() 
{
    Timer total_timer;
    Timer delta_timer;
    Timer cap_timer;

    int quit = false;
    uint32_t countedFrames = 0;
    float fps = 0.f;
    float deltaTime = 0;
    
    timer_start(&total_timer);
    timer_start(&delta_timer);
    timer_start(&cap_timer);

    while (!quit) {
        quit = handle_events();

        update();
        render();

        fps = countedFrames / (timer_get_ticks(&total_timer) / 1000.f);
        countedFrames++;

        int ticks = timer_get_ticks(&cap_timer);
        if (ticks < gamestate.ticks_per_frame) {
            SDL_Delay(gamestate.ticks_per_frame - ticks);
        }
        timer_start(&delta_timer);
        timer_start(&cap_timer);
    }
}

void close_app() 
{
    for (int i = 0; i < 1; i++) {
        if (entities[i]) {
            entities[i]->destroy(entities[i]);
        }
    }
    arena_reset(&arena);
    arena_destroy(&arena);

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	renderer = NULL;
	window = NULL;

	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

int main() 
{
    printf("start...\n");
    arena_create(&arena, ARENA_SIZE);

    if (initialize()) {
        Player* player = (Player*)arena_alloc(&arena, sizeof(Player));
        if (player == NULL) {
            printf("Unable to allocate player");
        }
        player_create(player, renderer);
        entities[0] = (Entity*)player;

        game_loop();
    }
    close_app();

    return 0;
}
