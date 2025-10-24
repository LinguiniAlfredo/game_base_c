#pragma once

typedef struct {
    char* buffer;
    size_t size;
    size_t offset;
} Arena;

void arena_create(Arena* arena, size_t size)
{
    printf("creating arena...\n");
    arena->buffer = malloc(size);
    arena->size = size;
    arena->offset = 0;
}

void* arena_alloc(Arena* arena, size_t size)
{
    printf("allocating arena...\n");
    if (arena->offset + size > arena->size) {
        //size_t new_size = ARENA_SIZE * 2;
        //char* tmp_buffer = realloc(arena->buffer, new_size);
        //if (tmp_buffer == NULL) 
        //    return NULL;
        //arena->buffer = tmp_buffer;
        //arena->size = new_size;
        return NULL;
    }

    void* ptr = arena->buffer + arena->offset;
    arena->offset += size;

    return ptr;
}

void arena_reset(Arena* arena) 
{
    printf("resetting arena...\n");
    arena->offset = 0;
}

void arena_destroy(Arena* arena)
{
    printf("destroying arena...\n");
    free(arena->buffer);
}
