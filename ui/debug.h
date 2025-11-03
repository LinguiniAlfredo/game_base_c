#pragma once

typedef struct FPSData {
    TTF_Font    *font;
    SDL_Texture *texture;
    SDL_Color    color;
    Vector2f     position;
} FPSData;

typedef struct MemoryData {
    int x;
} MemoryData;

typedef struct DebugMenu {
    FPSData    fps_data;
    //MemoryData memory_data;
} DebugMenu;

void debug_menu_create(DebugMenu *debug_menu)
{
    debug_menu->fps_data.color.r     = 0;
    debug_menu->fps_data.color.g     = 0;
    debug_menu->fps_data.color.b     = 0;
    debug_menu->fps_data.color.a     = 255;
    debug_menu->fps_data.position    = vector_create_zero();
    debug_menu->fps_data.font        = TTF_OpenFont("resources/fonts/nes.ttf", 7);
    debug_menu->fps_data.texture     = texture_create_text("FPS: 0", debug_menu->fps_data.font, debug_menu->fps_data.color, 5, 5);
}

void debug_menu_update(DebugMenu *debug_menu, float fps)
{
    if (debug_menu->fps_data.texture != NULL) {
        SDL_DestroyTexture(debug_menu->fps_data.texture);
    }
    char fps_label[] = "FPS: ";
    char fps_value[50];
    sprintf(fps_value, "%.2f", fps);
    strcat(fps_label, fps_value);
    debug_menu->fps_data.texture = texture_create_text(fps_label, debug_menu->fps_data.font, debug_menu->fps_data.color, 5, 5);
}

void debug_menu_destroy(DebugMenu *debug_menu)
{
    TTF_CloseFont(debug_menu->fps_data.font);
    SDL_DestroyTexture(debug_menu->fps_data.texture);
}

void debug_menu_render(DebugMenu *debug_menu)
{
    texture_render(debug_menu->fps_data.texture, vector_ftoi(debug_menu->fps_data.position));
}
