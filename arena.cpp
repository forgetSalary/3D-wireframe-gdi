#include "arena.h"

#define MAX(x, y) ((x) >= (y) ? (x) : (y))

#define BLOCK_PADDING (sizeof(ptr_node))

static void arena_grow(arena_t* arena, size_t min_size) {
    size_t size = ALIGN_UP(MAX(ARENA_BLOCK_SIZE+BLOCK_PADDING, min_size+BLOCK_PADDING), ARENA_ALIGNMENT);
    arena->ptr = (char*)malloc(size);
    memset(arena->ptr,0,ARENA_BLOCK_SIZE);
    arena->end = arena->ptr + size;
    list_append(&(arena->regions), (ptr_node*)arena->ptr);
    arena->ptr = (char*)ALIGN_UP_PTR(arena->ptr + BLOCK_PADDING, ARENA_ALIGNMENT);
}

void *arena_alloc(arena_t* arena, size_t size) {
    if (size > (size_t)(arena->end - arena->ptr)) {
        arena_grow(arena, size);
        assert(size <= (size_t)(arena->end - arena->ptr));
    }
    void *ptr = arena->ptr;
    arena->ptr = (char*)ALIGN_UP_PTR(arena->ptr + size, ARENA_ALIGNMENT);
    assert(arena->ptr <= arena->end);
    assert(ptr == ALIGN_DOWN_PTR(ptr, ARENA_ALIGNMENT));
    return ptr;
}

void* arena_dup(arena_t* arena,void* src,size_t size){
    void* dst = arena_alloc(arena,size);
    memcpy(dst,src,size);
    return dst;
}

void arena_free(arena_t* arena) {
    ptr_node* it_ptr = arena->regions.head;
    ptr_node it;
    while(it_ptr != arena->regions.tail){
        it = *it_ptr;
        free(it_ptr);
        it_ptr = it.next;
    }
    arena->ptr = NULL;
    arena->end = NULL;
    arena->regions.head = NULL;
    arena->regions.tail = NULL;
}
