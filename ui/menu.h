#pragma once

typedef struct MenuItem {
    SDL_Color    color;
    TTF_Font    *font;
    SDL_Texture *label;
    Vector2f     position;
    SDL_Rect     bounds;
    void (*action)();
} MenuItem;

typedef struct MainMenu {
    int x;
} MainMenu;

typedef struct PauseMenu {
    MenuItem title;
    MenuItem menu_items[3];
    int      selected_index;
} PauseMenu;

void return_to_game() { gamestate.mode = GAME; }
void quit_to_main()   { gamestate.mode = MENU; }
void exit_game()      { gamestate.mode = QUIT; }

void main_menu_create(MainMenu *main_menu)
{
    
}

void main_menu_handle_events(MainMenu *main_menu, SDL_Event *e)
{

}

void main_menu_render(MainMenu *main_menu)
{
    SDL_SetRenderDrawColor(gamestate.renderer, 0xF5, 0xF5, 0xF5, 0xFF);
    SDL_RenderClear(gamestate.renderer);

    // TODO - render main menu
 
    SDL_RenderPresent(gamestate.renderer);
}

void pause_create(PauseMenu *pause_menu)
{
    // TODO - calculate the width/height of label textures, use that to position instead of hardcoded numbers
    
    int screen_half_width  = gamestate.internal_screen_width / 2;
    int screen_half_height = gamestate.internal_screen_height / 2;

    pause_menu->selected_index = 0;
    pause_menu->title.color.r  = 0;
    pause_menu->title.color.g  = 0;
    pause_menu->title.color.b  = 0;
    pause_menu->title.color.a  = 255;
    pause_menu->title.font     = TTF_OpenFont("resources/fonts/boldpixels.ttf", 10);
    pause_menu->title.label    = texture_create_text("---paused---", pause_menu->title.font, pause_menu->title.color, 5, 5);
    pause_menu->title.position = vector_create(screen_half_width - 35, screen_half_height - 60);

    for (int i = 0; i < 3; i++) {
        pause_menu->menu_items[i].color.r  = 0;
        pause_menu->menu_items[i].color.g  = 0;
        pause_menu->menu_items[i].color.b  = 0;
        pause_menu->menu_items[i].color.a  = 255;
        pause_menu->menu_items[i].font     = TTF_OpenFont("resources/fonts/boldpixels.ttf", 10);
        pause_menu->menu_items[i].position = vector_create(screen_half_width + 40, screen_half_height + 20 * (i+1));
        switch (i) {
            case 0:
                pause_menu->menu_items[i].action = return_to_game;
                pause_menu->menu_items[i].label  = texture_create_text("return to game", pause_menu->menu_items[i].font, pause_menu->menu_items[i].color, 5, 5);
                break;
            case 1:
                pause_menu->menu_items[i].action = quit_to_main;
                pause_menu->menu_items[i].label  = texture_create_text("quit to main", pause_menu->menu_items[i].font, pause_menu->menu_items[i].color, 5, 5);
                break;
            case 2:
                pause_menu->menu_items[i].action = exit_game;
                pause_menu->menu_items[i].label  = texture_create_text("exit game", pause_menu->menu_items[i].font, pause_menu->menu_items[i].color, 5, 5);
                break;
                
        }

        // TODO - make into utility function to get size of texture
        int label_w = 0;
        int label_h = 0;
        SDL_QueryTexture(pause_menu->menu_items[i].label, NULL, NULL, &label_w, &label_h);
        SDL_Rect rect = { pause_menu->menu_items[i].position.x, pause_menu->menu_items[i].position.y, label_w, label_h };
        pause_menu->menu_items[i].bounds = rect;
    }
}

void pause_handle_events(PauseMenu *pause_menu, SDL_Event *e)
{
    if (e->type == SDL_KEYDOWN && e->key.repeat == 0) {
        SDL_Keycode key = e->key.keysym.sym;
        switch (key) {
            case SDLK_w:
                pause_menu->selected_index = (pause_menu->selected_index - 1) % 3;
                break;
            case SDLK_s:
                pause_menu->selected_index = (pause_menu->selected_index + 1) % 3;
                break;
            case SDLK_RETURN:
                pause_menu->menu_items[pause_menu->selected_index].action();
                break;
        }
    }
}

void pause_render(PauseMenu *pause_menu)
{
    SDL_SetRenderDrawColor(gamestate.renderer, 0xF5, 0xF5, 0xF5, 0xFF);
    SDL_RenderClear(gamestate.renderer);

    texture_render(pause_menu->title.label, vector_ftoi(pause_menu->title.position));
    for (int i = 0; i < 3; i++) {
        texture_render(pause_menu->menu_items[i].label, vector_ftoi(pause_menu->menu_items[i].position));
    }
    SDL_SetRenderDrawColor(gamestate.renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(gamestate.renderer, &pause_menu->menu_items[pause_menu->selected_index].bounds);

    SDL_RenderPresent(gamestate.renderer);
}

void pause_destroy(PauseMenu *pause_menu)
{
    TTF_CloseFont(pause_menu->title.font);
    SDL_DestroyTexture(pause_menu->title.label);

    for (int i = 0; i < 3; i++) {
        TTF_CloseFont(pause_menu->menu_items[i].font);
        SDL_DestroyTexture(pause_menu->menu_items[i].label);
    }
}
