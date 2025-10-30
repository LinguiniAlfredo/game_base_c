#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <stdint.h>
#include <limits.h>

#define ARENA_SIZE 1024
#define MAX_GAMEOBJECTS 5

#include "entities/types.h"
#include "utils/arena.h"
#include "utils/timer.h"
#include "gamestate.h"
#include "components/texture.h"
#include "components/animation.h"
#include "components/collision.h"
#include "ui/hud.h"
#include "entities/gameobject.h"
#include "entities/pickups/coin.h"
#include "entities/player.h"
#include "scenes/scene.h"

// TODO - Implement sound
//      - Implement collision detection (coin collection with sound)
//      - Create main menu
//      - Implement vector2 struct and use within player movement code
//      - Add more components to hud, always render this, separate out fps to debug only
//          - if we arena allocate it, it will get reallocated when scenes change, probably bad, unless we can store away the data 
//          - player score, lives remaining, etc. needs to persist across scene change
//      - Extra credit: make editor

Arena arena;
Scene *current_scene;

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

int handle_events()
{
    SDL_Event e;

    int quit = 0;
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT)
            quit = 1;

        if (e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym) {
                case SDLK_ESCAPE:
                    quit = 1;
                    break;
                case SDLK_1:
                    scene_change(current_scene, LEVEL1);
                    break;
                case SDLK_2:
                    scene_change(current_scene, LEVEL2);
                    break;
                case SDLK_F1:
                    toggle_debug();
                    break;
                default:
                    break;
            }
        }
        for (int i = 0; i < MAX_GAMEOBJECTS; i++) {
            GameObject *obj = current_scene->gameobjects[i];
            if (obj != NULL && obj->components & CONTROLLER) {
                obj->handle_events(obj, &e);
            }
        }

    }
    return quit;
}

void update_and_render(float delta_time, float fps, int current_frame)
{
    for (int i = 0; i < MAX_GAMEOBJECTS; i++) {
        GameObject *obj = current_scene->gameobjects[i];
        if (obj != NULL) {
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
            if (gamestate.debug && obj->components & COLLISION) {
                obj->render_collision(obj);
            }
            if (obj->components & TEXTURE) {
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

    int quit = 0;
    uint64_t current_frame = 0;
    float fps = 0;
    float delta_time = 0;

    timer_start(&total_timer);
    timer_start(&fps_cap_timer);

    while (!quit) {
        fps = current_frame / (timer_get_ticks(&total_timer) / 1000.f);
        current_frame++;

        quit = handle_events();

        update_and_render(delta_time, fps, current_frame);

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
        current_scene = (Scene *)arena_alloc(&gamestate.arena, sizeof(Scene));
        if (current_scene == NULL)
            printf("Unable to allocate scene\n");
        scene_create(current_scene, LEVEL1);

        game_loop();
    }
    close_app();

    return 0;
}
