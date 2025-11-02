#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

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
#include "ui/hud.h"
#include "ui/menu.h"
#include "entities/gameobject.h"
#include "entities/player.h"
#include "entities/pickups/coin.h"
#include "scenes/scene.h"
#include "input/input.h"

// TODO - Implement sound
//      - Create main menu
//      - Add more components to hud, always render this, separate out fps to debug only
//          - if we arena allocate it, it will get reallocated when scenes change, probably bad, unless we can store away the data 
//          - player score, lives remaining, etc. needs to persist across scene change
//      - Extra credit: make editor

Arena      arena;
Scene      *current_scene;
MainMenu   *main_menu;
PauseMenu  *pause_menu;
SDL_Window *window = NULL;

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
                    gamestate.mode = QUIT;
                    break;
                case SDLK_1:
                    scene_change(current_scene, LEVEL1);
                    break;
                case SDLK_2:
                    scene_change(current_scene, LEVEL2);
                    break;
                case SDLK_TAB:
                    toggle_paused();
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
                main_menu_handle_events(main_menu, &e);
                break;
            case GAME: {
                GameObject *player = current_scene->gameobjects[0];
                if (player != NULL)
                    player->handle_events(player, &e);
            } break;
            case PAUSED: {
                pause_handle_events(pause_menu, &e);
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
        GameObject *obj = current_scene->gameobjects[i];
        if (obj != NULL && obj->alive) {
            obj->update(obj, delta_time, current_frame);
            if (obj->components & COLLISION) {
                obj->handle_collision(obj, current_scene->gameobjects);
            }
        }
    }

    SDL_SetRenderDrawColor(gamestate.renderer, 0xF5, 0xF5, 0xF5, 0xFF);
    SDL_RenderClear(gamestate.renderer);

    for (int i = 0; i < MAX_GAMEOBJECTS; i++) {
        GameObject *obj = current_scene->gameobjects[i];
        if (obj != NULL) {
            if (gamestate.debug && obj->components & COLLISION && obj->alive) {
                obj->render_collision(obj);
            }
            if (obj->components & TEXTURE && obj->alive) {
                obj->render(obj);
            }
        }
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
                main_menu_render(main_menu);
                break;
            case GAME:
                update_and_render(delta_time, fps, current_frame);
                break;
            case PAUSED:
                pause_render(pause_menu);
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
    pause_destroy(pause_menu);

    for (int i = 0; i < MAX_GAMEOBJECTS; i++) {
        GameObject *obj = current_scene->gameobjects[i];
        if (obj != NULL) {
            obj->destroy(obj);
        }
    }

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
    gamestate.arena = arena;
    arena_create(&gamestate.arena, ARENA_SIZE);

    if (initialize() == 0) {
        // load menus into memory
        pause_menu = (PauseMenu *)arena_alloc(&gamestate.arena, sizeof(PauseMenu));
        if (pause_menu == NULL)
            printf("Unable to load pause menu\n");
        pause_create(pause_menu);

        // load scene into memory
        current_scene = (Scene *)arena_alloc(&gamestate.arena, sizeof(Scene));
        if (current_scene == NULL)
            printf("Unable to allocate scene\n");
        scene_create(current_scene, LEVEL1);

        game_loop();
    }
    close_app();

    return 0;
}
