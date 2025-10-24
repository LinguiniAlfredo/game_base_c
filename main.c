#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <stdint.h>

#include "gamestate.h"
#include "components/texture.h"
#include "components/hud.h"
#include "entities.h"
#include "utils/arena.h"
#include "utils/timer.h"

#define ARENA_SIZE 1000
#define MAX_ENTITIES (ARENA_SIZE / sizeof(Entity))

Arena arena;
Entity *entities[MAX_ENTITIES];
Hud *hud;

SDL_Renderer *renderer = NULL;
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

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("renderer could not be created : %s", SDL_GetError());
        return 1;
    }
    SDL_RenderSetLogicalSize(renderer, gamestate.internal_screen_width,
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
                    break;
                case SDLK_F1:
                    toggle_debug();
                    break;
                default:
                    break;
            }
        }

        for (int i = 0; i < MAX_ENTITIES; i++) {
            if (entities[i]) {
                entities[i]->handle_events(entities[i], &e);
            }
        }
    }
    return quit;
}

void update(float delta_time, float fps)
{
    hud_update(hud, fps, renderer);
    for (int i = 0; i < MAX_ENTITIES; i++) {
        if (entities[i]) {
            entities[i]->update(entities[i], delta_time);
        }
    }
}

void render()
{
    SDL_SetRenderDrawColor(renderer, 0xF5, 0xF5, 0xF5, 0xFF);
    SDL_RenderClear(renderer);

    if (gamestate.debug)
        hud_render(hud, renderer);

    for (int i = 0; i < MAX_ENTITIES; i++) {
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

    int quit = 0;
    uint32_t counted_frames = 0;
    float fps = 0;
    float delta_time = 0;

    timer_start(&total_timer);
    timer_start(&delta_timer);

    while (!quit) {
        fps = counted_frames / (timer_get_ticks(&total_timer) / 1000.f);
        counted_frames++;

        quit = handle_events();

        update(delta_time, fps);
        render();

        int ticks = timer_get_ticks(&delta_timer);
        if (ticks < gamestate.ticks_per_frame)
            SDL_Delay(gamestate.ticks_per_frame - ticks);

        delta_time = ticks / 1000.f;
        timer_start(&delta_timer);
    }
}

void close_app()
{
    hud_destroy(hud);
    for (int i = 0; i < MAX_ENTITIES; i++) {
        if (entities[i])
            entities[i]->destroy(entities[i]);
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

    if (initialize() == 0) {

        hud = (Hud *)arena_alloc(&arena, sizeof(Hud));
        if (hud == NULL)
            printf("Unable to allocate hud");
        hud_create(hud, renderer);

        Player *player = (Player *)arena_alloc(&arena, sizeof(Player));
        if (player == NULL)
            printf("Unable to allocate player");
        player_create(player, renderer);
        entities[0] = (Entity *)player;

        game_loop();
    }
    close_app();

    return 0;
}
