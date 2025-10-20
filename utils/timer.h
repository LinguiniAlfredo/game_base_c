typedef struct {
    uint32_t start_ticks;
    uint32_t paused_ticks;
    int paused;
    int started;
} Timer;

void timer_start(Timer* timer) 
{
    timer->started = 1;
    timer->paused = 0;

    timer->start_ticks = SDL_GetTicks();
    timer->paused_ticks = 0;
}

void timer_stop(Timer* timer) 
{
    timer->started = 0;
    timer->paused = 0;
    
    timer->start_ticks = 0;
    timer->paused_ticks = 0;
}

void timer_pause(Timer* timer) 
{
    if (timer->started && timer->paused == 0) {
        timer->paused = 1;
        timer->paused_ticks = SDL_GetTicks() - timer->start_ticks;
        timer->start_ticks = 0;
    }
}

void timer_unpause(Timer* timer) 
{
    if (timer->started && timer->paused) {
        timer->paused = 0;
        timer->start_ticks = SDL_GetTicks() - timer->paused_ticks;
        timer->paused_ticks = 0;
    }
}

uint32_t timer_get_ticks(Timer* timer) 
{
    uint32_t time = 0;

    if (timer->started) {
        if (timer->paused) {
            time = timer->paused_ticks;
        } else {
            time = SDL_GetTicks() - timer->start_ticks;
        }
    }

    return time;
}
