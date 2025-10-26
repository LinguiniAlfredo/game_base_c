
typedef struct Hud {
    TTF_Font *font;
    SDL_Texture *fps_texture;
    SDL_Color color;
    int fps_pos_x, fps_pos_y;
} Hud; // 

void hud_render(Hud *hud)
{
    texture_render(gamestate.renderer, hud->fps_texture, hud->fps_pos_x, hud->fps_pos_y);
}

void hud_update(Hud *hud, float fps)
{
    if (hud->fps_texture != NULL) {
        SDL_DestroyTexture(hud->fps_texture);
    }
    char fps_label[] = "FPS: ";
    char fps_value[50];
    sprintf(fps_value, "%.2f", fps);
    strcat(fps_label, fps_value);
    hud->fps_texture = texture_create_text(gamestate.renderer, fps_label, hud->font, hud->color, 5, 5);
}

void hud_create(Hud *hud)
{
    hud->fps_pos_x = 0;
    hud->fps_pos_y = 0;
    hud->color.r = 0;
    hud->color.g = 0;
    hud->color.b = 0;
    hud->color.a = 255;

    hud->font = TTF_OpenFont("resources/fonts/nes.ttf", 10);
    hud->fps_texture = texture_create_text(gamestate.renderer, "FPS: 0", hud->font, hud->color, 5, 5);
}

void hud_destroy(Hud *hud)
{
    TTF_CloseFont(hud->font);
    SDL_DestroyTexture(hud->fps_texture);
}
