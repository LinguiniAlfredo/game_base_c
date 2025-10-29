#pragma once

#define MAX_GAMEOBJECTS 5

typedef enum Level {
    MAINMENU,
    LEVEL1,
    LEVEL2,
    LEVEL3
} Level;

typedef struct Scene {
    GameObject *gameobjects[MAX_GAMEOBJECTS];
    Level level;
} Scene;

void scene_create(Scene *scene, Level level)
{
    switch (level) {
        case MAINMENU:
            break;

        case LEVEL1: {
            Player *player = (Player *)arena_alloc(&gamestate.arena, sizeof(Player));
            if (player == NULL)
                printf("Unable to allocate player\n");
            player_create(player);
            scene->gameobjects[0] = (GameObject *)player;

            Coin *coin = (Coin *)arena_alloc(&gamestate.arena, sizeof(Coin));
            if (coin == NULL)
                printf("Unable to allocate coin\n");
            coin_create(coin);
            scene->gameobjects[1] = (GameObject *)coin;
        } break;

        case LEVEL2: {
            Coin *coin = (Coin *)arena_alloc(&gamestate.arena, sizeof(Coin));
            if (coin == NULL)
                printf("Unable to allocate coin\n");
            coin_create(coin);
            scene->gameobjects[0] = (GameObject *)coin;
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
        if (scene->gameobjects[i] != NULL) {
            scene->gameobjects[i]->destroy(scene->gameobjects[i]);
        }
    }
}

void scene_change(Scene *scene, Level level)
{
    arena_reset(&gamestate.arena);
    scene_destroy(scene);

    scene = (Scene *)arena_alloc(&gamestate.arena, sizeof(Scene));
    if (scene == NULL)
        printf("Unable to allocate scene\n");
    scene_create(scene, level);
}

