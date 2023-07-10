#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

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

PtrTreeMap_Entry __TreeMapEntry_find(
    PtrTreeMap_Entry self,
    int depth,
    uint8_t isBfs,
    int (*visit)(PtrTreeMap_Entry node, int depth))
{
    if (self == NULL)
        return NULL;

    if (isBfs && visit(self, depth))
    {
        return self;
    }

    PtrTreeMap_Entry res = NULL;
    if (self->__left != NULL)
    {
        res = __TreeMapEntry_find(self->__left, depth + 1, isBfs, visit);
    }
    if (res == NULL && self->__right != NULL)
    {
        res = __TreeMapEntry_find(self->__right, depth + 1, isBfs, visit);
    }

    if (!isBfs && visit(self, depth))
    {
        return self;
    }

    return res;
}

void _printNode(PtrTreeMap_Entry self)
{
    if (self == NULL)
    {
        printf("\n");
        return;
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

PtrTreeMap_Entry _get_uncle(PtrTreeMap_Entry self)
{
    PtrTreeMap_Entry papa, grandpapa;
    if (self->__parent != NULL)
    {
        papa = self->__parent;
        grandpapa = papa->__parent;
        if (grandpapa != NULL)
        {
            if (grandpapa->__right == papa)
            {
                return grandpapa->__left;
            }
            else
            {
                return grandpapa->__right;
            }
        }
    }
    return NULL;
}

int __TreeMap_dump_node(PtrTreeMap_Entry self, int depth)
{
    for (int i = 0; i < depth; ++i)
    {
        printf("-|");
    }
    _printNode(self);
    return 0;
}

void _dump_tree(PtrTreeMap_Entry node)
{
    if (node == NULL)
        return;
    // find root
    PtrTreeMap_Entry root = node;
    while (root->__parent != NULL)
    {
        root = root->__parent;
    }
    // print tree
    __TreeMapEntry_find(root, 0, 1, __TreeMap_dump_node);
}

void _fix_node(PtrTreeMap_Entry node)
{
    PtrTreeMap_Entry uncle = _get_uncle(node);
    printf("Fixing invalid node [%p]:", node);
    _printNode(node);
    printf("Uncle [%p]", uncle);
    _printNode(uncle);
    uint8_t ucolor = TreeMap_Entry_BLACK;
    if (uncle != NULL)
    {
        ucolor = uncle->__color;
    }
    printf("Tree BEFORE >>>>>>>>>>>>>>>>\n");
    _dump_tree(node);
    // If uncle is red we recolor
    if (ucolor == TreeMap_Entry_RED)
    {
        node->__color = TreeMap_Entry_RED;
        node->__parent->__color = TreeMap_Entry_BLACK;
        uncle->__color = TreeMap_Entry_BLACK;
        // if grandpa isnt root set it red
        if (uncle->__parent->__parent != NULL)
        {
            uncle->__parent->__color = TreeMap_Entry_RED;
        }
    }
    // if uncle is black we rebalance
    printf("Tree AFTER >>>>>>>>>>>>>>>>\n");
    _dump_tree(node);
}

void balance(PtrTreeMap_Entry node)
{
    if (node == NULL)
        return;

    // Check the __color of the node
    if (node->__parent != NULL)
    {
        // (TREE PORPERTY):  No red node has a red child
        if (node->__color == TreeMap_Entry_RED && node->__parent->__color == TreeMap_Entry_RED)
        {
            _fix_node(node);
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
        self->__count++;
    }
    else
    {
        printf("Error allocating memory for treemap entry\n");
    }
}

int __TreeMap_get(struct TreeMap *self, char *key)
{
}

void __TreeMap_dump(PtrTreeMap self)
{
    _dump_tree(self->__root);
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

////////////////// TESTS //////////////////////////
int __TreeMap_test_root_is_black(PtrTreeMap self)
{
    return self->__root == NULL || self->__root->__color == TreeMap_Entry_BLACK;
}

int __TreeMap_find_red_red_node(PtrTreeMap_Entry self, int depth)
{
    if (self != NULL && self->__color == TreeMap_Entry_RED)
    {
        if (self->__left != NULL && self->__left->__color == TreeMap_Entry_RED)
        {
            printf("[ERROR]: Node { %s } violates [Property 2]\n", self->__left->key);
            return 1;
        }
        if (self->__right != NULL && self->__right->__color == TreeMap_Entry_RED)
        {
            printf("[ERROR]: Node { %s } violates [Property 2]\n", self->__right->key);
            return 1;
        }
    }
    return 0;
}
int __TreeMap_test_no_red_node_red_child(PtrTreeMap self)
{
    PtrTreeMap_Entry res = __TreeMapEntry_find(self->__root, 0, 0, __TreeMap_find_red_red_node);
    return res == NULL;
}

int __TreeMap_test_each_path_has_same_num_black_nodes(PtrTreeMap self)
{
    // Reset counts just for now
    // This forumalation, of this algorithm, could potentially be implemented with find
    // and globals, however it was fun to write this implementation anyway
    int pass = 1;
    int max_height = (int)log2((double)self->__count) + 2;
    PtrTreeMap_Entry *path_buf = malloc(sizeof(*path_buf) * max_height);
    PtrTreeMap_Entry *head = path_buf;
    *path_buf = self->__root;
    
    int blacks_per_path = 0;
    while (head >= path_buf)
    {
        // pop it
        PtrTreeMap_Entry node = *head;
        head--;
        // push it
        if (node->__right != NULL)
        {
            head++;
            *head = node->__right;
        }
        if (node->__left != NULL)
        {
            head++;
            *head = node->__left;
        }
        // finish it
        if (node->__right == NULL && node->__left == NULL)
        {
            int black_counter = 0;
            PtrTreeMap_Entry h = node;
            while (h != NULL)
            {
                if (h->__color == TreeMap_Entry_BLACK)
                {
                    black_counter ++;
                }
                h = h->__parent;
            }
            printf("Leaf found %s #[%d]\n", node->key, black_counter);
            if(!blacks_per_path) {
                blacks_per_path = black_counter;
            } else if (blacks_per_path != black_counter) {
                pass = 0;
            }
        }
    }
    free(path_buf);
    return pass;
}

void __TreeMap_test_tree(PtrTreeMap self)
{
    printf("[Property 1] Root is always black .......................... %s\n",
           __TreeMap_test_root_is_black(self) ? "[PASS]" : "[FAIL]");
    printf("[Property 2] No red node has a red child ................... %s\n",
           __TreeMap_test_no_red_node_red_child(self) ? "[PASS]" : "[FAIL]");
    printf("[Property 3] Every path has same number of black nodes ..... %s\n",
           __TreeMap_test_each_path_has_same_num_black_nodes(self) ? "[PASS]" : "[FAIL]");
}
////////////////// END TESTS //////////////////////

int main(void)
{
    printf("Testing TreeMap\n");

    srand(12345678);
    PtrTreeMap map = TreeMap_new();
    if (map)
    {
        for (int i = 0; i < 20; ++i)
        {
            int k = rand();
            char v[2];
            v[0] = 'a' + (k % 26); // random character
            map->put(map, v, k);
        }
    }
    printf("\n\n\n\nRESULT:\n");
    map->dump(map);
    printf("\n\n");
    __TreeMap_test_tree(map);
    // map->free(map);
    return EXIT_SUCCESS;
}
