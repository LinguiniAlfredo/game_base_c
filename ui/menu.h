#pragma once

typedef struct MenuItem {
    SDL_Color    color;
    TTF_Font    *font;
    SDL_Texture *label;
    Vector2f     position;
    void (*action)();
} MenuItem;

typedef struct MainMenu {
    int x;
} MainMenu;

typedef struct PauseMenu {
    MenuItem title;
    MenuItem return_to_game;
    MenuItem quit_to_main;
    MenuItem exit_game;
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

}

void pause_create(PauseMenu *pause_menu)
{
    // TODO - calculate the width/height of label textures, use that to position instead of hardcoded numbers
    
    int screen_half_width  = gamestate.internal_screen_width / 2;
    int screen_half_height = gamestate.internal_screen_height / 2;

    pause_menu->title.color.r  = 0;
    pause_menu->title.color.g  = 0;
    pause_menu->title.color.b  = 0;
    pause_menu->title.color.a  = 255;
    pause_menu->title.font     = TTF_OpenFont("resources/fonts/boldpixels.ttf", 10);
    pause_menu->title.label    = texture_create_text("---paused---", pause_menu->title.font, pause_menu->title.color, 5, 5);
    pause_menu->title.position = vector_create(screen_half_width - 35, screen_half_height - 60);

    pause_menu->return_to_game.color.r  = 0;
    pause_menu->return_to_game.color.g  = 0;
    pause_menu->return_to_game.color.b  = 0;
    pause_menu->return_to_game.color.a  = 255;
    pause_menu->return_to_game.font     = TTF_OpenFont("resources/fonts/boldpixels.ttf", 10);
    pause_menu->return_to_game.label    = texture_create_text("return to game", pause_menu->return_to_game.font, pause_menu->return_to_game.color, 5, 5);
    pause_menu->return_to_game.position = vector_create(screen_half_width + 40, screen_half_height + 20);
    pause_menu->return_to_game.action   = return_to_game;

    pause_menu->quit_to_main.color.r    = 0;
    pause_menu->quit_to_main.color.g    = 0;
    pause_menu->quit_to_main.color.b    = 0;
    pause_menu->quit_to_main.color.a    = 255;
    pause_menu->quit_to_main.font       = TTF_OpenFont("resources/fonts/boldpixels.ttf", 10);
    pause_menu->quit_to_main.label      = texture_create_text("quit to main menu", pause_menu->quit_to_main.font, pause_menu->quit_to_main.color, 5, 5);
    pause_menu->quit_to_main.position   = vector_create(screen_half_width + 40, screen_half_height + 40);
    pause_menu->quit_to_main.action     = quit_to_main;

    pause_menu->exit_game.color.r       = 0;
    pause_menu->exit_game.color.g       = 0;
    pause_menu->exit_game.color.b       = 0;
    pause_menu->exit_game.color.a       = 255;
    pause_menu->exit_game.font          = TTF_OpenFont("resources/fonts/boldpixels.ttf", 10);
    pause_menu->exit_game.label         = texture_create_text("exit game", pause_menu->exit_game.font, pause_menu->exit_game.color, 5, 5);
    pause_menu->exit_game.position      = vector_create(screen_half_width + 40, screen_half_height + 60);
    pause_menu->exit_game.action        = exit_game;
}

void pause_handle_events(PauseMenu *pause_menu, SDL_Event *e)
{

}

void pause_render(PauseMenu *pause_menu)
{
    SDL_SetRenderDrawColor(gamestate.renderer, 0xF5, 0xF5, 0xF5, 0xFF);
    SDL_RenderClear(gamestate.renderer);

    texture_render(pause_menu->title.label, vector_ftoi(pause_menu->title.position));
    texture_render(pause_menu->return_to_game.label, vector_ftoi(pause_menu->return_to_game.position));
    texture_render(pause_menu->quit_to_main.label, vector_ftoi(pause_menu->quit_to_main.position));
    texture_render(pause_menu->exit_game.label, vector_ftoi(pause_menu->exit_game.position));

    SDL_RenderPresent(gamestate.renderer);
}

void pause_destroy(PauseMenu *pause_menu)
{
    TTF_CloseFont(pause_menu->title.font);
    SDL_DestroyTexture(pause_menu->title.label);
    TTF_CloseFont(pause_menu->return_to_game.font);
    SDL_DestroyTexture(pause_menu->return_to_game.label);
    TTF_CloseFont(pause_menu->quit_to_main.font);
    SDL_DestroyTexture(pause_menu->quit_to_main.label);
    TTF_CloseFont(pause_menu->exit_game.font);
    SDL_DestroyTexture(pause_menu->exit_game.label);
}
