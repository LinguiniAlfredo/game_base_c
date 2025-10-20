#pragma once

SDL_Texture* texture_create(SDL_Renderer* renderer, char* filepath) 
{
    SDL_Texture* new_texture;
    SDL_Surface* surface = IMG_Load(filepath);
    if (surface == NULL) {
		printf("Unable to load image %s : %s\n", filepath, IMG_GetError());
    } else {
        new_texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (new_texture == NULL) {
			printf("Unable to create texture %s : %s\n", filepath, SDL_GetError());
        }
        SDL_FreeSurface(surface);
    }
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
