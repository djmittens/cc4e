#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct TreeMap_Entry
{
    char *key;
    int value;
    struct TreeMap_Entry *__next;
    struct TreeMap_Entry *__left;
    struct TreeMap_Entry *__right;
} *PtrTreeMap_Entry;

typedef struct TreeMap_Iter
{
    PtrTreeMap_Entry head;
    PtrTreeMap_Entry (*next)(struct TreeMap_Iter *self);
    void (*free)(struct TreeMap_Iter *self);
} *PtrTreeMap_Iter;

void __TreeMap_Iter_free(PtrTreeMap_Iter self)
{
    free(self);
}

PtrTreeMap_Entry __TreeMap_Iter_next(PtrTreeMap_Iter self)
{
    PtrTreeMap_Entry tmp = self->head;
    if (tmp != NULL)
    {
        self->head = tmp->__next;
    }
    return tmp;
}

typedef struct TreeMap
{
    int __count;
    PtrTreeMap_Entry __head;
    PtrTreeMap_Entry __root;
    void (*put)(struct TreeMap *self, char *key, int value);
    int (*get)(struct TreeMap *self, char *key);
    int (*size)(struct TreeMap *self);
    void (*free)(struct TreeMap *self);
    void (*dump)(struct TreeMap *self);
    PtrTreeMap_Iter (*iter)(struct TreeMap *self);
} *TreeMapPtr;

TreeMapPtr TreeMap_new()
{
    TreeMapPtr res = malloc(sizeof(*res));
    res->__head = NULL;
    void __TreeMap_put(struct TreeMap * self, char *key, int value);
    int __TreeMap_get(struct TreeMap * self, char *key);
    int __TreeMap_size(struct TreeMap * self);
    PtrTreeMap_Iter __TreeMap_iter(struct TreeMap * self);
    void __TreeMap_dump(TreeMapPtr self);
    void __TreeMap_free(TreeMapPtr self);
    res->put = __TreeMap_put;
    res->get = __TreeMap_get;
    res->size = __TreeMap_size;
    res->iter = __TreeMap_iter;
    res->free = __TreeMap_free;
    res->dump = __TreeMap_dump;
    return res;
}

void __TreeMap_free(TreeMapPtr self)
{
    PtrTreeMap_Iter iter = self->iter(self);

    for (PtrTreeMap_Entry head = iter->next(iter); head != NULL; head = iter->next(iter))
    {
        free(head);
    }
    iter->free(iter);
    free(self);
}

void __TreeMap_put(struct TreeMap *self, char *key, int value)
{
    PtrTreeMap_Entry tmp = self->__head;
    PtrTreeMap_Entry new = malloc(sizeof(*new));
    int klen = strnlen(key, 10);
    new->key = malloc(klen + 1);
    strncpy(new->key, key, klen);
    new->value = value;
    new->__next = tmp;
    new->__left = tmp;
    self->__head = new;
}

int __TreeMap_get(struct TreeMap *self, char *key)
{
}

void __TreeMap_dump(TreeMapPtr self)
{
    printf("Hello World !!!!");
}
int __TreeMap_size(struct TreeMap *self)
{
    return self->__count;
}
PtrTreeMap_Iter __TreeMap_iter(struct TreeMap *self)
{
    PtrTreeMap_Iter res = malloc(sizeof(*res));
    res->next = __TreeMap_Iter_next;
    res->head = self->__head;
    res->free = __TreeMap_Iter_free;
    return res;
}

int main(void)
{
    printf("Testing TreeMap\n");
    TreeMapPtr map = TreeMap_new();
    map->put(map, "h", 42);
    map->put(map, "d", 8);
    map->put(map, "f", 5);
    map->put(map, "b", 123);
    map->put(map, "k", 9);
    map->put(map, "m", 67);
    map->put(map, "j", 12);
    map->put(map, "f", 6);

    map->dump(map);
    map->free(map);
    return EXIT_SUCCESS;
}

