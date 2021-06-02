#ifndef ARENA_H
#define ARENA_H

#include "framework.h"
#include <assert.h>

struct ptr_node{
    struct ptr_node* next;
};

struct ptr_list{
    ptr_node* head;
    ptr_node* tail;
};

struct arena_t{
    char *ptr;
    char *end;
    ptr_list regions;
};

#define ARENA_ALIGNMENT sizeof(uintptr_t)
#define ARENA_BLOCK_SIZE 4096

//операции над списками
#define list_init(l,ptr) ((l)->tail = (l)->head = (ptr),(l)->head->next = NULL)
#define list_append(l,n) ((((l)->tail) && ((l)->head)) ? \
                         (n)->next = NULL,(l)->tail->next = (n),(l)->tail = (n) : list_init(l,n))

//макросы для варвнивания данных
#define ALIGN_DOWN(n, a) ((n) & ~((a) - 1))
#define ALIGN_UP(n, a) ALIGN_DOWN((n) + (a) - 1, (a))

#define ALIGN_DOWN_PTR(p, a) ((void *)ALIGN_DOWN((uintptr_t)(p), (a)))
#define ALIGN_UP_PTR(p, a) ((void *)ALIGN_UP((uintptr_t)(p), (a)))

#define EMPTY_ARENA  {NULL,NULL,{NULL,NULL}}

void *arena_alloc(arena_t* arena, size_t size);

void arena_free(arena_t* arena);

void* arena_dup(arena_t* arena,void* src,size_t size);

#endif //ALLOCS_ARENA_H
