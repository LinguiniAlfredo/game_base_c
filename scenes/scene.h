#pragma once

#include "../utils/arena.h"
#include "../entities/gameobject.h"
#include "../entities/player.h"
#include "../entities/coin.h"
#include "../gamestate.h"
#include "../utils/vector.h"

typedef enum Level {
    LEVEL1,
    LEVEL2,
    LEVEL3
} Level;

// TODO - load levels intelligently from map file or something
typedef struct Scene {
    Level       level;
    //Map         map;
} Scene;

void scene_create(Scene *scene, Level level)
{
    switch (level) {
        case LEVEL1: {
            // load level1 map
            Player *player = (Player *)arena_alloc(&gamestate.arena, ENTITY, sizeof(Player));
            player_create(player);
            gamestate.gameobjects[0] = (GameObject *)player;

            int center_screen_x = gamestate.internal_screen_width / 2;

            for (int i = 0; i < 5; i++) {
                Coin *coin = (Coin *)arena_alloc(&gamestate.arena, ENTITY, sizeof(Coin));
                coin_create(coin, vector_create(center_screen_x + (i * 15), 30));
                gamestate.gameobjects[i + 1] = (GameObject *)coin;
            }
        } break;

        case LEVEL2: {
            Coin *coin = (Coin *)arena_alloc(&gamestate.arena, ENTITY, sizeof(Coin));
            coin_create(coin, vector_create(5, 5));
            gamestate.gameobjects[0] = (GameObject *)coin;
        } break;

        case LEVEL3:
            break;

        default:
            break;
    }
}

void scene_destroy(Scene *scene)
{
    for (int i = 0; i < MAX_GAMEOBJECTS; i++) {
        if (gamestate.gameobjects[i] != NULL) {
            gamestate.gameobjects[i]->destroy(gamestate.gameobjects[i]);
        }
    }
}

void scene_load(Scene **scene, Level level)
{
    arena_reset_part(&gamestate.arena, ENTITY);
    if (*scene != NULL)
        scene_destroy(*scene);

    *scene = (Scene *)arena_alloc(&gamestate.arena, ENTITY, sizeof(Scene));
    scene_create(*scene, level);
}

