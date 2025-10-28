#pragma once

typedef enum Components {
    COLLISION  = 1,
    TEXTURE    = 2,
    ANIMATION  = 4,
    PHYSICS    = 8,
    CONTROLLER = 16

} Components;

typedef struct GameObject {
    int   alive;
    float pos_x, pos_y;
    float speed;
    float vel_x, vel_y;
    Components components;

    void (*handle_events)(struct GameObject*, SDL_Event *e);
    void (*update)(struct GameObject*, float delta_time, int current_frame);
    void (*render)(struct GameObject*);
    void (*render_collision)(struct GameObject*);
    void (*destroy)(struct GameObject*);
    
} GameObject;
