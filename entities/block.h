#pragma once

typedef struct Block {
    GameObject   base;
    SDL_Texture *spritesheet;
} Block;

void block_create(Block *block)
{
    block->base.components       = COLLISION | TEXTURE;

    block->base.position         = vector_create((gamestate.internal_screen_width / 2) - 10, (gamestate.internal_screen_height / 2) - 10);
    block->base.speed            = 0;
    block->base.velocity         = vector_create_zero();
    block->base.alive            = 1;
    block->base.solid            = 1;
    block->base.update           = coin_update;
    block->base.render           = coin_render;
    block->base.render_collision = coin_render_collision;
    block->base.handle_collision = coin_handle_collision;
    block->base.destroy          = coin_destroy;

    block->animation             = animation_create();
    block->spritesheet           = texture_create("resources/spritesheets/coin.png");
    block->collision             = collision_create(coin->base.position.x, coin->base.position.y, 8, 8);
}
