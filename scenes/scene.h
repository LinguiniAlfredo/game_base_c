#pragma once

typedef enum Level {
    MAINMENU,
    LEVEL1,
    LEVEL2,
    LEVEL3
} Level;

typedef struct Scene {
    GameObject *gameobjects[MAX_GAMEOBJECTS];
    Level       level;
} Scene;

void scene_create(Scene *scene, Level level)
{
    // TODO - load levels intelligently from map file or something
    switch (level) {
        case MAINMENU:
            break;

        case LEVEL1: {
            Player *player = (Player *)arena_alloc(&gamestate.arena, ENTITY, sizeof(Player));
            if (player == NULL)
                printf("Unable to allocate player\n");
            player_create(player);
            scene->gameobjects[0] = (GameObject *)player;

            Coin *coin = (Coin *)arena_alloc(&gamestate.arena, ENTITY, sizeof(Coin));
            if (coin == NULL)
                printf("Unable to allocate coin\n");
            coin_create(coin);
            scene->gameobjects[1] = (GameObject *)coin;
        } break;

        case LEVEL2: {
            Coin *coin = (Coin *)arena_alloc(&gamestate.arena, ENTITY, sizeof(Coin));
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
    arena_reset_part(&gamestate.arena, ENTITY);
    scene_destroy(scene);

    scene = (Scene *)arena_alloc(&gamestate.arena, ENTITY, sizeof(Scene));
    if (scene == NULL)
        printf("Unable to allocate scene\n");
    scene_create(scene, level);
}

