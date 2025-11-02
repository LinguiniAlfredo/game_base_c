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
    MenuItem title;
    MenuItem menu_items[2];
    int selected_index;
} MainMenu;

typedef struct PauseMenu {
    MenuItem title;
    MenuItem menu_items[3];
    int      selected_index;
} PauseMenu;

void play_game() {
    gamestate.mode = GAME;
}
void return_to_game() { gamestate.mode = GAME; }
void quit_to_main()   { gamestate.mode = MENU; }
void exit_game()      { gamestate.mode = QUIT; }

// TODO - create global menu_create function that takes in some set of parameters for the menu and creates it, removing specialized functions for each menu

void main_menu_create(MainMenu *main_menu)
{

    int screen_half_width  = gamestate.internal_screen_width / 2;
    int screen_half_height = gamestate.internal_screen_height / 2;

    main_menu->selected_index = 0;
    main_menu->title.color.r  = 0;
    main_menu->title.color.g  = 0;
    main_menu->title.color.b  = 0;
    main_menu->title.color.a  = 255;
    main_menu->title.font     = TTF_OpenFont("resources/fonts/boldpixels.ttf", 10);
    main_menu->title.label    = texture_create_text("Game Title", main_menu->title.font, main_menu->title.color, 5, 5);
    main_menu->title.position = vector_create(screen_half_width - 35, screen_half_height - 60);

    for (int i = 0; i < 2; i++) {
        main_menu->menu_items[i].color.r  = 0;
        main_menu->menu_items[i].color.g  = 0;
        main_menu->menu_items[i].color.b  = 0;
        main_menu->menu_items[i].color.a  = 255;
        main_menu->menu_items[i].font     = TTF_OpenFont("resources/fonts/boldpixels.ttf", 10);
        main_menu->menu_items[i].position = vector_create(screen_half_width + 40, screen_half_height + 20 * (i+1));
        switch (i) {
            case 0:
                main_menu->menu_items[i].action = play_game;
                main_menu->menu_items[i].label  = texture_create_text("play game", main_menu->menu_items[i].font, main_menu->menu_items[i].color, 5, 5);
                break;
            case 1:
                main_menu->menu_items[i].action = exit_game;
                main_menu->menu_items[i].label  = texture_create_text("exit game", main_menu->menu_items[i].font, main_menu->menu_items[i].color, 5, 5);
                break;
                
        }

        Vector2i label_size = texture_measure(main_menu->menu_items[i].label);
        SDL_Rect rect = { main_menu->menu_items[i].position.x - 2.5, main_menu->menu_items[i].position.y - 2.5, label_size.x + 5, label_size.y + 5 };
        main_menu->menu_items[i].bounds = rect;
    }
}

void main_menu_alloc(MainMenu **main_menu)
{
    *main_menu = (MainMenu *)arena_alloc(&gamestate.arena, UI, sizeof(MainMenu));
    if (*main_menu == NULL)
        printf("Unable to load main menu\n");
    main_menu_create(*main_menu);
}

void pause_create(PauseMenu *pause_menu)
{
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
                pause_menu->menu_items[i].label  = texture_create_text("resume", pause_menu->menu_items[i].font, pause_menu->menu_items[i].color, 5, 5);
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

        Vector2i label_size = texture_measure(pause_menu->menu_items[i].label);
        SDL_Rect rect = { pause_menu->menu_items[i].position.x - 2.5, pause_menu->menu_items[i].position.y - 2.5, label_size.x + 5, label_size.y + 5 };
        pause_menu->menu_items[i].bounds = rect;
    }
}

void pause_menu_alloc(PauseMenu **pause_menu)
{
    *pause_menu = (PauseMenu *)arena_alloc(&gamestate.arena, UI, sizeof(PauseMenu));
    if (*pause_menu == NULL)
        printf("Unable to load pause menu\n");
    pause_create(*pause_menu);
}

// TODO - create global menu_handle_events that takes in number of menu items that can be used for any type of menu
void main_menu_handle_events(MainMenu *main_menu, SDL_Event *e)
{
    if (e->type == SDL_KEYDOWN && e->key.repeat == 0) {
        SDL_Keycode key = e->key.keysym.sym;
        switch (key) {
            case SDLK_w:
                main_menu->selected_index = (main_menu->selected_index - 1) % 2;
                break;
            case SDLK_s:
                main_menu->selected_index = (main_menu->selected_index + 1) % 2;
                break;
            case SDLK_RETURN:
                main_menu->menu_items[main_menu->selected_index].action();
                break;
        }
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

void main_menu_render(MainMenu *main_menu)
{
    SDL_SetRenderDrawColor(gamestate.renderer, 0xF5, 0xF5, 0xF5, 0xFF);
    SDL_RenderClear(gamestate.renderer);

    texture_render(main_menu->title.label, vector_ftoi(main_menu->title.position));
    for (int i = 0; i < 2; i++) {
        texture_render(main_menu->menu_items[i].label, vector_ftoi(main_menu->menu_items[i].position));
    }
    SDL_SetRenderDrawColor(gamestate.renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(gamestate.renderer, &main_menu->menu_items[main_menu->selected_index].bounds);
 
    SDL_RenderPresent(gamestate.renderer);
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

void main_menu_destroy(MainMenu *main_menu)
{
    TTF_CloseFont(main_menu->title.font);
    SDL_DestroyTexture(main_menu->title.label);

    for (int i = 0; i < 2; i++) {
        TTF_CloseFont(main_menu->menu_items[i].font);
        SDL_DestroyTexture(main_menu->menu_items[i].label);
    }
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
