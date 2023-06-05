#include <stdio.h>
#include <stdlib.h>

typedef struct lnode
{
    int value;
    struct lnode *next;
    struct lnode *prev;
} LNODE;

LNODE *new_list()
{
    LNODE *list = malloc(sizeof(LNODE));
    list->value = -1;
    list->prev = NULL;
    list->next = NULL;
    return list;
}

int free_list(LNODE *list)
{
    while (list != NULL)
    {
        printf("Freeing: %d\n", list->value);
        LNODE *n = list;
        list = n->next;
        free(n);
    }
}

void push(LNODE *list, int value)
{
    printf("Pushing: %d\n", value);

    LNODE *h = list->next;
    LNODE *n = malloc(sizeof(LNODE));
    // empty list
    n->value = value;
    n->prev = list;
    // Non empty list
    if (h != NULL)
    {
        h->prev = n;
        n->next = h;
    }
    // Empty list
    else
    {
        n->next = NULL;
    }
    list->next = n;
}

void remove_node(LNODE *node)
{
    if(node == NULL) return;

    LNODE * prev = node->prev;
    if (node->next != NULL)
    {
        node->next->prev = prev;
        prev->next = node->next;
    }
    free(node);
}

int pop(LNODE *list)
{
    if (list->next == NULL)
    {
        return -1;
    }

    LNODE *n = list->next;
    int res = n->value;
    remove_node(n);
    return res;
}

LNODE *find(LNODE *list, int value)
{
    while (list != NULL && list->value != value)
    {
        list = list->next;
    }
    return list;
}


void remove_value(LNODE *list, int value)
{
    remove_node(find(list, value));
}

int main()
{
    LNODE *list = new_list();

    for (int i = 0; i < 50; ++i)
    {
        push(list, i);
    }
    
    remove_value(list, 2);
    remove_value(list, 43);
    pop(list);

    free_list(list);

    return EXIT_SUCCESS;
}