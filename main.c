#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
//#include <SDL2/SDL_mixer.h>

#include <stdint.h>
#include <limits.h>
#include <math.h>

#define ARENA_SIZE 1024
#define MAX_GAMEOBJECTS 5

#include "entities/types.h"
#include "utils/arena.h"
#include "utils/timer.h"
#include "utils/vector.h"
#include "gamestate.h"
#include "components/texture.h"
#include "components/animation.h"
#include "components/collision.h"
#include "entities/gameobject.h"
#include "entities/player.h"
#include "entities/pickups/coin.h"
#include "scenes/scene.h"
#include "input/input.h"
#include "ui/menu.h"
#include "ui/debug.h"
#include "ui/hud.h"
#include "ui/ui.h"

// TODO - Implement sound
//      - Debug mode shows arena memory layout and fps on-screen
//      - Play game menu item should re-load Level1
//      - Add hud items (lives, coins, etc.)
//      - Add enemy type
//      - Clean up includes, every header should include everything it needs to compile on its own, nothing more
//      *** Editor and map loading will be game-specific, so not in this base

SDL_Window *window = NULL;

Gamestate gamestate = {
    .mode                   = MENU,
    .debug                  = 0,
    .gameover               = 0,
    .tile_size              = 16,
    .screen_width           = 1920/2,
    .screen_height          = 1080/2,
    .internal_screen_width  = 320,
    .internal_screen_height = 180,
    .ticks_per_frame        = 1000.f / 60,
    .tile_width             = 320 / 16,
    .tile_height            = 180 / 16,
};

int initialize()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    window = SDL_CreateWindow("", SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED, gamestate.screen_width,
                                gamestate.screen_height, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    gamestate.renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (gamestate.renderer == NULL) {
        printf("renderer could not be created : %s", SDL_GetError());
        return 1;
    }
    SDL_RenderSetLogicalSize(gamestate.renderer, gamestate.internal_screen_width,
                            gamestate.internal_screen_height);

    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        printf("SDL_Image could not init : %s", IMG_GetError());
        return 1;
    }
    if (TTF_Init() == -1) {
        printf("SDL_TTF failed to initialize: %s\n", TTF_GetError());
    }

    return 0;
}

void toggle_debug() { gamestate.debug = !gamestate.debug; }

void toggle_paused() { 
    if (gamestate.mode != PAUSED) {
        gamestate.mode = PAUSED;
    } else {
        gamestate.mode = GAME;
    }
}

void handle_events()
{
    SDL_Event e;

    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT)
            gamestate.mode = QUIT;

        if (e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym) {
                case SDLK_ESCAPE:
                    if (gamestate.mode == GAME) {
                        toggle_paused();
                    }
                    break;
                case SDLK_1:
                    scene_load(&gamestate.current_scene, LEVEL1);
                    break;
                case SDLK_2:
                    scene_load(&gamestate.current_scene, LEVEL2);
                    break;
                case SDLK_F1:
                    toggle_debug();
                    break;
                default:
                    break;
            }
        }

        switch (gamestate.mode) {
            case MENU:
                main_menu_handle_events(gamestate.ui->main_menu, &e);
                break;
            case GAME: {
                GameObject *player = gamestate.current_scene->gameobjects[0];
                if (player != NULL)
                    player->handle_events(player, &e);
            } break;
            case PAUSED: {
                pause_handle_events(gamestate.ui->pause_menu, &e);
            } break;
            case EDIT:
                break;
            case QUIT:
                break;
        }
    }
}

void update_and_render(float delta_time, float fps, int current_frame)
{
    for (int i = 0; i < MAX_GAMEOBJECTS; i++) {
        GameObject *obj = gamestate.current_scene->gameobjects[i];
        if (obj != NULL && obj->alive) {
            obj->update(obj, delta_time, current_frame);
            if (obj->components & COLLISION) {
                obj->handle_collision(obj, gamestate.current_scene->gameobjects);
            }
        }
    }

    SDL_SetRenderDrawColor(gamestate.renderer, 0xF5, 0xF5, 0xF5, 0xFF);
    SDL_RenderClear(gamestate.renderer);

    for (int i = 0; i < MAX_GAMEOBJECTS; i++) {
        GameObject *obj = gamestate.current_scene->gameobjects[i];
        if (obj != NULL) {
            if (gamestate.debug && obj->components & COLLISION && obj->alive) {
                obj->render_collision(obj);
            }
            if (obj->components & TEXTURE && obj->alive) {
                obj->render(obj);
            }
        }
    }

    if (gamestate.debug) {
        debug_menu_update(gamestate.ui->debug_menu, fps);
        debug_menu_render(gamestate.ui->debug_menu);
    }

    SDL_RenderPresent(gamestate.renderer);
}

void game_loop()
{
    Timer total_timer;
    Timer fps_cap_timer;
    uint64_t current_frame = 0;
    float fps = 0;
    float delta_time = 0;

    timer_start(&total_timer);
    timer_start(&fps_cap_timer);

    while (gamestate.mode != QUIT) {
        fps = current_frame / (timer_get_ticks(&total_timer) / 1000.f);
        current_frame++;

        handle_events();

        switch (gamestate.mode) {
            case MENU:
                main_menu_render(gamestate.ui->main_menu);
                break;
            case GAME:
                update_and_render(delta_time, fps, current_frame);
                break;
            case PAUSED:
                pause_render(gamestate.ui->pause_menu);
                break;
            case EDIT:
                break;
            case QUIT:
                break;
        }

        int ticks = timer_get_ticks(&fps_cap_timer);
        if (ticks < gamestate.ticks_per_frame)
            SDL_Delay(gamestate.ticks_per_frame - ticks);

        if (fps > 0)
            delta_time = 1 / fps;

        timer_start(&fps_cap_timer);
    }
}

void close_app()
{
    ui_destroy(gamestate.ui);
    scene_destroy(gamestate.current_scene);

    arena_reset(&gamestate.arena);
    arena_destroy(&gamestate.arena);

    SDL_DestroyRenderer(gamestate.renderer);
    SDL_DestroyWindow(window);
    gamestate.renderer = NULL;
    window = NULL;

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

int main()
{
    arena_create(&gamestate.arena, ARENA_SIZE);

    if (initialize() == 0) {
        // load subsystems into arena memory
        ui_load(&gamestate.ui);
        // sound_load(&sound);

        game_loop();
    }
    close_app();

    return 0;
}
