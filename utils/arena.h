#pragma once

typedef enum {
    UI,
    ENTITY
} Subsystem;

typedef struct {
    char  *buffer;
    size_t size;
    size_t offset;
} Partition;

typedef struct {
    Partition ui_partition;
    Partition entity_partition;
} Arena;

void arena_create(Arena* arena, const size_t size)
{
    arena->ui_partition.buffer = malloc(size);
    arena->ui_partition.size   = size / 2;
    arena->ui_partition.offset = 0;

    arena->entity_partition.buffer = arena->ui_partition.buffer + arena->ui_partition.size;
    arena->entity_partition.size   = size / 2;
    arena->entity_partition.offset = 0;
}

void* arena_alloc(Arena* arena, const Subsystem partition, const size_t size)
{
    size_t part_offset, part_size;
    char*  part_buffer;

    switch (partition) {
        case UI: {
            part_offset = arena->ui_partition.offset;
            part_size   = arena->ui_partition.size;
            part_buffer = arena->ui_partition.buffer;
            arena->ui_partition.offset += size;
            printf("allocating from ui partition\n");
        } break;
        case ENTITY: {
            part_offset = arena->entity_partition.offset;
            part_size   = arena->entity_partition.size;
            part_buffer = arena->entity_partition.buffer;
            arena->entity_partition.offset += size;
            printf("allocating from entity partition\n");
        } break;
    }

    if (part_offset + size > part_size) {
        printf("ran out of space in partition...\n");
        return NULL;
    }

    return part_buffer + part_offset;
}

void arena_reset_part(Arena* arena, const Subsystem partition) 
{
    switch (partition) {
        case UI:
            printf("resetting ui partition\n");
            arena->ui_partition.offset = 0;
            break;
        case ENTITY:
            printf("resetting entity partition\n");
            arena->entity_partition.offset = 0;
            break;
    }
}

void arena_reset(Arena* arena)
{
    printf("resetting all partitions\n");
    arena->ui_partition.offset = 0;
    arena->entity_partition.offset = 0;
}

void arena_destroy(Arena* arena)
{   // TODO - pointer to ui is technically pointer to entire arena
    //        is this good? should i have a separate pointer to entire arena
    printf("destroying arena...\n");
    free(arena->ui_partition.buffer);
}

void arena_debug(Arena *arena)
{
    
}

