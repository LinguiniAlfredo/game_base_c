#pragma once
#include <SDL2/SDL_mixer.h>

typedef enum SOUND {
    SOUND_COIN,
    SOUND_HURT,
    SOUND_PAUSE,
    SOUND_SELECT
} SOUND;

void sound_start(SOUND sound) {
    Mix_PlayChannel(-1, gamestate.sounds[sound], 0);
}

void sound_destroy()
{
    for (int i = 0; i < MAX_SOUNDS; i++) {
        if (gamestate.sounds[i] != NULL) {
            Mix_FreeChunk(gamestate.sounds[i]);
        }
    }
}

void sound_load()
{
    gamestate.sounds[0] = Mix_LoadWAV("resources/sounds/coin.wav");
    gamestate.sounds[1] = Mix_LoadWAV("resources/sounds/hurt.wav");
    gamestate.sounds[2] = Mix_LoadWAV("resources/sounds/pause.wav");
    gamestate.sounds[3] = Mix_LoadWAV("resources/sounds/menu_select.wav");
}
