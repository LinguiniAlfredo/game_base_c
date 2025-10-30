#pragma once

typedef struct Collision {
    SDL_Rect bounds;
} Collision;

Collision collision_create(int x, int y, int w, int h)
{
    Collision collision;

    collision.bounds.x = x;
    collision.bounds.y = y;
    collision.bounds.w = w;
    collision.bounds.h = h;

    return collision;
}

void collision_render(Collision *collision)
{
    SDL_SetRenderDrawColor(gamestate.renderer, 0, 255, 0, 255);
    SDL_RenderDrawRect(gamestate.renderer, &collision->bounds);
}

int collision_detect(SDL_Rect collision1, SDL_Rect collision2)
{
    if (SDL_HasIntersection(&collision1, &collision2)) {
        return 1;
    } else {
        return 0;
    }
}
