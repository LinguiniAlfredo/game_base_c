#pragma once

typedef struct Ui {
    Hud       *hud;
    MainMenu  *main_menu;
    PauseMenu *pause_menu;
    DebugMenu *debug_menu;
} Ui;

void ui_create(Ui *ui)
{
    ui->main_menu  =  (MainMenu *)arena_alloc(&gamestate.arena, UI, sizeof(MainMenu));
    ui->pause_menu = (PauseMenu *)arena_alloc(&gamestate.arena, UI, sizeof(PauseMenu));
    ui->debug_menu = (DebugMenu *)arena_alloc(&gamestate.arena, UI, sizeof(DebugMenu));
    ui->hud        =       (Hud *)arena_alloc(&gamestate.arena, UI, sizeof(Hud));

    main_menu_create(ui->main_menu);
    pause_create(ui->pause_menu);
    debug_menu_create(ui->debug_menu);
    hud_create(ui->hud);
}

void ui_load(Ui **ui)
{
    *ui = (Ui *)arena_alloc(&gamestate.arena, UI, sizeof(Ui));
    ui_create(*ui);
}

void ui_destroy(Ui *ui)
{
    main_menu_destroy(ui->main_menu);
    pause_destroy(ui->pause_menu);
    debug_menu_destroy(ui->debug_menu);
    hud_destroy(ui->hud);
}
