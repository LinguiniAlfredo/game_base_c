#pragma once
#include <SDL2/SDL_mixer.h>

typedef enum SOUND {
    SOUND_COIN,
    SOUND_HURT,
    SOUND_PAUSE,
    SOUND_SELECT
} SOUND;

typedef enum MUSIC {
    MUSIC_LEVEL1
} MUSIC;

void sound_start(SOUND sound)
{
    Mix_PlayChannel(-1, gamestate.sounds[sound], 0);
}

void music_start(MUSIC music)
{
    if (Mix_PlayingMusic() == 1) {
        Mix_HaltMusic();
    }
    Mix_PlayMusic(gamestate.music[music], -1);
}

void music_stop()
{
    if (Mix_PlayingMusic() == 1) {
        Mix_HaltMusic();
    }
}

void music_pause()
{
    if (Mix_PlayingMusic() == 1) {
        Mix_PauseMusic();
    }
}

void music_resume()
{
    if (Mix_PausedMusic() == 1) {
        Mix_ResumeMusic();
    }
}

void sound_destroy()
{
    for (int i = 0; i < MAX_SOUNDS; i++) {
        if (gamestate.sounds[i] != NULL) {
            Mix_FreeChunk(gamestate.sounds[i]);
        }
    }
}

void music_destroy()
{
    for (int i = 0; i < MAX_MUSIC; i++) {
        if (gamestate.music[i] != NULL) {
            Mix_FreeMusic(gamestate.music[i]);
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

void music_load()
{
    Mix_VolumeMusic(64); // 50% TODO - create menu to control this
    gamestate.music[0] = Mix_LoadMUS("resources/music/level1.mp3");
    if (gamestate.music[0] == NULL) {
        printf("error loading music file");
    }
}
