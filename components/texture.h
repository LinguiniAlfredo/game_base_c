#pragma once

SDL_Texture* texture_create(SDL_Renderer* renderer, char* filepath)
{
    SDL_Texture* new_texture;
    SDL_Surface* surface = IMG_Load(filepath);

    if (surface == NULL) {
		printf("Unable to load image %s : %s\n", filepath, IMG_GetError());
        return NULL;
    }
    new_texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (new_texture == NULL)
        printf("Unable to create texture %s : %s\n", filepath, SDL_GetError());

    SDL_FreeSurface(surface);
    return new_texture;
}

SDL_Texture* texture_create_text(SDL_Renderer *renderer, char *text, TTF_Font *font, SDL_Color color, int width, int height)
{
    SDL_Texture *new_texture;
    SDL_Surface *surface = TTF_RenderText_Solid(font, text, color);
    if (surface == NULL) {
        printf("Unable to load text %s\n", TTF_GetError());
        return NULL;
    }

    new_texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (new_texture == NULL) {
        printf("Unable to create texture from text\n");
        return NULL;
    }

    SDL_FreeSurface(surface);
    return new_texture;
}

void texture_render(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y)
{
    int tex_w = 0;
    int tex_h = 0;

    SDL_QueryTexture(texture, NULL, NULL, &tex_w, &tex_h);
    SDL_Rect renderQuad = { x, y, tex_w, tex_h };
    SDL_RenderCopy(renderer, texture, NULL, &renderQuad);
}

void texture_render_clipped(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y, SDL_Rect stencil)
{
    SDL_Rect renderQuad = { x, y, stencil.w, stencil.h };
    SDL_RenderCopy(renderer, texture, &stencil, &renderQuad);
}
