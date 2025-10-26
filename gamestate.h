#pragma once

typedef struct {
    int debug;
    int gameover;
    int tile_size;
    int screen_width;
    int screen_height;
    int internal_screen_height;
    int internal_screen_width;
    int tile_width;
    int tile_height;

    float ticks_per_frame;

    SDL_Renderer *renderer;

} Gamestate;

Gamestate gamestate = {
    .debug = 0,
    .gameover = 0,
    .tile_size = 16,
    .screen_width = 1920 / 2,
    .screen_height = 1080 / 2,
    .internal_screen_width = 320,
    .internal_screen_height = 180,
    .ticks_per_frame = 1000.f / 60,
    .tile_width = 320 / 16,
    .tile_height = 180 / 16
};

Gamestate gamestate_create() 
{
    // TODO - put renderer and window in here too so i dont have to pass them aroudn
    printf("creating gamestate...");
    Gamestate gs = {
        .debug = 0,
        .gameover = 0,
        .tile_size = 16,
        .screen_width = 1920 / 2,
        .screen_height = 1080 / 2,
        .internal_screen_width = 320,
        .internal_screen_height = 180,
        .ticks_per_frame = 1000.f / 60
    };
    gs.tile_width = gs.internal_screen_width / gs.tile_size;
    gs.tile_height = gs.internal_screen_height / gs.tile_size;

    return gs;
}

