#pragma once

SDL_Texture* texture_create(char *filepath)
{
    SDL_Texture* new_texture;
    SDL_Surface* surface = IMG_Load(filepath);

    if (surface == NULL) {
		printf("unable to load image %s : %s\n", filepath, IMG_GetError());
        return NULL;
    }
    new_texture = SDL_CreateTextureFromSurface(gamestate.renderer, surface);
    if (new_texture == NULL)
        printf("unable to create texture %s : %s\n", filepath, SDL_GetError());

    SDL_FreeSurface(surface);
    return new_texture;
}

SDL_Texture* texture_create_text(char *text, TTF_Font *font, SDL_Color color, int width, int height)
{
    SDL_Texture *new_texture;
    SDL_Surface *surface = TTF_RenderText_Solid(font, text, color);
    if (surface == NULL) {
        printf("unable to load text %s\n", TTF_GetError());
        return NULL;
    }

    new_texture = SDL_CreateTextureFromSurface(gamestate.renderer, surface);
    if (new_texture == NULL) {
        printf("unable to create texture from text\n");
        return NULL;
    }

    SDL_FreeSurface(surface);
    return new_texture;
}

Vector2f texture_measure(SDL_Texture *texture)
{
    int tex_w = 0;
    int tex_h = 0;
    SDL_QueryTexture(texture, NULL, NULL, &tex_w, &tex_h);
    return vector_create(tex_w, tex_h);
}

float texture_get_screen_center_x(SDL_Texture *texture)
{
    Vector2f size = texture_measure(texture);
    return (gamestate.internal_screen_width / 2) - (size.x / 2);
}

float texture_get_screen_center_y(SDL_Texture *texture)
{
    Vector2f size = texture_measure(texture);
    return (gamestate.internal_screen_height / 2) - (size.y / 2);
}

void texture_render(SDL_Texture *texture, Vector2i position)
{
    Vector2i size = vector_ftoi(texture_measure(texture));
    SDL_Rect renderQuad = { position.x, position.y, size.x, size.y };
    SDL_RenderCopy(gamestate.renderer, texture, NULL, &renderQuad);
}

void texture_render_clipped(SDL_Texture *texture, Vector2i position, SDL_Rect stencil)
{
    SDL_Rect renderQuad = { position.x, position.y, stencil.w, stencil.h };
    SDL_RenderCopy(gamestate.renderer, texture, &stencil, &renderQuad);
}
