#pragma once

enum ComponentsMask {
    COLLISION  = 0x1,
    TEXTURE    = 0x2,
    ANIMATION  = 0x4,
    PHYSICS    = 0x8,
    CONTROLLER = 0x10
};

typedef struct GameObject {
    unsigned char alive;
    float pos_x, pos_y;
    float speed;
    float vel_x, vel_y;
    uint8_t components;

    void (*handle_events)(struct GameObject*, SDL_Event *e);
    void (*update)(struct GameObject*, float delta_time, int current_frame);
    void (*render)(struct GameObject*);
    void (*render_collision)(struct GameObject*);
    void (*destroy)(struct GameObject*);
    
} GameObject;
