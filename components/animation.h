#pragma once

typedef struct Animation {
    int num_frames;
    int playing;
    int duration;
    SDL_Rect stencil;

} Animation;

void animation_start(Animation *animation, int num_frames)
{
    animation->playing = 1;
    animation->stencil.x = 0;
    animation->num_frames = num_frames;
}

void animation_pause(Animation *animation)
{
}

void animation_stop(Animation *animation)
{
    animation->playing = 0;
}

void animation_update(Animation *animation, int current_frame)
{
    if (current_frame % 10 == 0) {
        animation->stencil.x += animation->stencil.w;
        if (animation->stencil.x >= animation->stencil.w * animation->num_frames) {
            animation->stencil.x = 0;
            animation_stop(animation);
        }
    }
}

void animation_set_frame(Animation *animation, int frame)
{
    animation->stencil.x = animation->stencil.w * frame;
}

void animation_destroy(Animation *animation)
{
}

Animation animation_create() // TODO - parameterize animation properties
{
    Animation animation;
    
    animation.num_frames = 4;
    animation.playing = 0;
    animation.duration = 0;
    animation.stencil.x = 0;
    animation.stencil.y = 0;
    animation.stencil.w = 8;
    animation.stencil.h = 8;

    return animation;
}
