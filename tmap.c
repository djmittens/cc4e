#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

const uint8_t TreeMap_Entry_BLACK = 0;
const uint8_t TreeMap_Entry_RED = 1;

typedef struct TreeMap_Entry
{
    char *key;
    int value;
    uint8_t __color;
    struct TreeMap_Entry *__next, *__parent, *__left, *__right;
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
} TreeMap, *PtrTreeMap;

PtrTreeMap TreeMap_new()
{
    PtrTreeMap res = malloc(sizeof(*res));
    if (res)
    {
        res->__count = 0;
        res->__head = NULL;
        res->__root = NULL;
        void __TreeMap_put(struct TreeMap * self, char *key, int value);
        int __TreeMap_get(struct TreeMap * self, char *key);
        int __TreeMap_size(struct TreeMap * self);
        void __TreeMap_dump(PtrTreeMap self);
        void __TreeMap_free(PtrTreeMap self);
        PtrTreeMap_Iter __TreeMap_iter(struct TreeMap * self);
        res->put = __TreeMap_put;
        res->get = __TreeMap_get;
        res->size = __TreeMap_size;
        res->free = __TreeMap_free;
        res->dump = __TreeMap_dump;
        res->iter = __TreeMap_iter;
    }
    else
    {
        printf("Could not allocate the map itself\n");
    }
    return res;
}

void __TreeMap_free(PtrTreeMap self)
{
    PtrTreeMap_Iter iter = self->iter(self);

    for (PtrTreeMap_Entry head = iter->next(iter); head != NULL; head = iter->next(iter))
    {
        free(head);
    }
    iter->free(iter);
    free(self);
}

void __TreeMap_insertNode(PtrTreeMap_Entry root, PtrTreeMap_Entry node)
{
    if (strncmp(root->key, node->key, 100) > 0) // TOOD(nick): MAX count here
    {
        if (root->__right == NULL)
        {
            node->__color = TreeMap_Entry_RED;
            node->__parent = root;
            root->__right = node;
        }
        else
        {
            __TreeMap_insertNode(root->__right, node);
        }
    }
    else
    {
        if (root->__left == NULL)
        {
            node->__color = TreeMap_Entry_RED;
            node->__parent = root;
            root->__left = node;
        }
        else
        {
            __TreeMap_insertNode(root->__left, node);
        }
    }
}

void fix_node(PtrTreeMap_Entry node)
{
    printf("Fixing invalid node [%p]: %s\n", node, node->key);
    // If aunt is black we recolor
    // if aunt is read we rebalance

}

void balance(PtrTreeMap_Entry node)
{
    if (node == NULL)
        return;

    // Check the __color of the node
    if (node->__parent != NULL)
    {
        if (node->__color == TreeMap_Entry_RED && node->__parent->__color == TreeMap_Entry_RED)
        {
            fix_node(node);
        }
    }

    balance(node->__parent);
}

void __TreeMap_put(PtrTreeMap self, char *key, int value)

{
    PtrTreeMap_Entry tmp = self->__head;
    PtrTreeMap_Entry new = malloc(sizeof(*new));
    if (new)
    {
        int klen = strnlen(key, 100); // TODO(nick) : make this a constant
        new->key = malloc(klen + 1);  // Maybe this alloc should be zeroed ?
        if (new->key)
        {
            strncpy(new->key, key, klen + 1);
        }
        else
        {
            printf("could not allocate key buffer\n");
            new->key = NULL;
        }
        new->value = value;
        new->__next = tmp;
        new->__parent = NULL;
        new->__left = NULL;
        new->__right = NULL;                // TODO (nick) : there was a bug here, for uninitilized memory, how do we catch this?
        new->__color = TreeMap_Entry_BLACK; // By default we also start as black
        self->__head = new;
        if (self->__root == NULL)
        {
            self->__root = new;
        }
        else
        {
            __TreeMap_insertNode(self->__root, new);
            balance(new);
        }
    }
    else
    {
        printf("Error allocating memory for treemap entry\n");
    }
}

PtrTreeMap_Entry __TreeMapEntry_traverse(
    PtrTreeMap_Entry self,
    int depth,
    uint8_t isBfs,
    int (*visit)(PtrTreeMap_Entry node, int depth))
{
    if (self == NULL)
        return NULL;

    if (isBfs)
    {
        visit(self, depth);
    }

    PtrTreeMap_Entry res = NULL;
    if (self->__left != NULL)
    {
        res = __TreeMapEntry_traverse(self->__left, depth + 1, isBfs, visit);
    }
    if (self->__right != NULL)
    {
        res = __TreeMapEntry_traverse(self->__right, depth + 1, isBfs, visit);
    }

    if (!isBfs)
    {
        visit(self, depth);
    }

    return res;
}

int __TreeMap_get(struct TreeMap *self, char *key)
{
}

int __TreeMap_dump_node(PtrTreeMap_Entry self, int depth)
{
    if (self != NULL)
    {
        for (int i = 0; i < depth; ++i)
        {
            printf("-|");
        }

        if (self->__color == TreeMap_Entry_BLACK)
        {
            printf(" [BLACK]");
        }
        else
        {
            printf(" [RED]");
        }
        printf(" %s -> %d\n", self->key, self->value);
    }
    return 0;
}
void __TreeMap_dump(PtrTreeMap self)
{
    __TreeMapEntry_traverse(self->__root, 0, 1, __TreeMap_dump_node);
}
int __TreeMap_size(struct TreeMap *self)
{
    return self->__count;
}
PtrTreeMap_Iter __TreeMap_iter(struct TreeMap *self)
{
    PtrTreeMap_Iter res = malloc(sizeof((*res)));
    res->next = __TreeMap_Iter_next;
    res->head = self->__head;
    res->free = __TreeMap_Iter_free;
    return res;
}

int main(void)
{
    printf("Testing TreeMap\n");

    PtrTreeMap map = TreeMap_new();
    if (map)
    {
        map->put(map, "h", 42);
        map->put(map, "d", 8);
        // map->put(map, "f", 5);
        map->put(map, "b", 123);
        map->put(map, "k", 9);
        map->put(map, "m", 67);
        map->put(map, "j", 12);
        map->put(map, "f", 6);

        map->dump(map);
    }
    // map->free(map);
    return EXIT_SUCCESS;
}
