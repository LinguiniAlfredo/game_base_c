#pragma once
#include <SDL2/SDL.h>

typedef struct Scene Scene;
typedef struct Ui    Ui;

typedef enum {
    MENU,
    GAME,
    PAUSED,
    EDIT,
    QUIT
} Gamemode;

typedef struct Gamestate {
    SDL_Renderer *renderer;
    Arena         arena;
    Ui           *ui;
    Scene        *current_scene;
    Gamemode      mode;
    int           debug;
    int           gameover;
    int           tile_size;
    int           screen_width;
    int           screen_height;
    int           internal_screen_height;
    int           internal_screen_width;
    int           tile_width;
    int           tile_height;
    float         ticks_per_frame;

} Gamestate;

extern Gamestate gamestate;
