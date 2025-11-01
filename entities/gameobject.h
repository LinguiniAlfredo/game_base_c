#pragma once

enum ComponentsMask {
    COLLISION  = 0x1,
    TEXTURE    = 0x2,
    ANIMATION  = 0x4,
    PHYSICS    = 0x8,
    CONTROLLER = 0x10
};

typedef struct GameObject {
    EntityType    type;
    Vector2f      position;
    Vector2f      velocity;
    float         speed;
    uint8_t       components;
    unsigned char alive;
    unsigned char solid;

    void (*handle_events)(struct GameObject*, SDL_Event*);
    void (*update)(struct GameObject*, float, int);
    void (*render)(struct GameObject*);
    void (*render_collision)(struct GameObject*);
    void (*handle_collision)(struct GameObject*, struct GameObject *gameobjects[]);
    void (*destroy)(struct GameObject*);
    
} GameObject;
