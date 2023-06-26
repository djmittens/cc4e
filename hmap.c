#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef struct MapEntry
{
    char *key;
    int value;
    struct MapEntry *next;
} *MapEntryPtr;

typedef struct HashMap
{
    int __count;
    int __nbuckets;
    MapEntryPtr *__heads;

    void (*put)(struct HashMap *self, char *key, int value);
    int (*get)(struct HashMap *self, char *key, int _default);
    // void (*del)(struct HashMap* self, char *key); TODO(nick): I dont feel like it.
    int (*size)(struct HashMap *self);
    void (*dump)(struct HashMap *self);
    // HashMapIterPtr (*iter)(struct HashMap *self, char *key);
} *HashMapPtr;

uint32_t __hash(char * key) {
    static const uint32_t A = 54059;
    static const uint32_t B = 76963;
    char * head = key;
    uint32_t res = 37;
    while(head != NULL && *head != '\0') {
        res = (res * A) ^ (*head * B);
        head ++;
    }
    return res;
}

HashMapPtr new_HashMap(int nbuckets)
{
    HashMapPtr res = malloc(sizeof(*res));
    // properties
    res->__nbuckets = nbuckets;
    res->__count = 0;
    // alloc buckets
    res->__heads = malloc(sizeof(MapEntryPtr) * nbuckets);
    for (int i = 0; i < nbuckets; ++i)
    {
        res->__heads[i] = NULL;
    }
    // interface
    void HashMap_put(struct HashMap * self, char *key, int value);
    int HashMap_get(struct HashMap * self, char *key, int _default);
    int HashMap_size(struct HashMap * self);
    void HashMap_dump(struct HashMap * self);
    res->put = HashMap_put;
    res->get = HashMap_get;
    res->size = HashMap_size;
    res->dump = HashMap_dump;
    return res;
}

MapEntryPtr __new_MapEntry(char *key, int val)
{
    static const int MAX_KEY_LEN = 100;
    MapEntryPtr res = malloc(sizeof(*res));
    // int ds = strnlen_s(key, MAX_KEY_LEN) + 1; // account for null byte
    // res->key = malloc(ds);
    // strncpy_s(res->key, ds, key, MAX_KEY_LEN);
    res->key = strndup(key, MAX_KEY_LEN);
    res->value = val;
    res->next = NULL;
    return res;
}

void HashMap_put(struct HashMap *self, char *key, int value)
{
    int bucket = __hash(key) % self->__nbuckets;
    MapEntryPtr head = self->__heads[bucket];
    MapEntryPtr entry = __new_MapEntry(key, value);
    if (head == NULL)
    {
        // Init a new value in there
        self->__heads[bucket] = entry;
    }
    else
    {
        entry->next = head;
        self->__heads[bucket] = entry;
    }
    self->__count ++;
}
int HashMap_get(struct HashMap *self, char *key, int _default)
{
    int bucket = __hash(key) % self->__nbuckets;
    for (MapEntryPtr head = self->__heads[bucket]; head != NULL; head = head->next)
    {
        if(strcmp(key, head->key) == 0) {
            return head->value;
        }
    }
    return _default;
}
int HashMap_size(struct HashMap *self)
{
    return self->__count;
}
void HashMap_dump(struct HashMap *self)
{
    printf("Object HashMap count=%d buckets=%d\n", self->size(self), self->__nbuckets);
    for (int i = 0; i < self->__nbuckets; ++i)
    {
        MapEntryPtr head = self->__heads[i];
        while (head != NULL)
        {
            printf("[%d] %s == %d\n", i, head->key, head->value);
            head = head->next;
        }
    }
}

int main()
{
    printf("Hello World!!!");
    HashMapPtr map = new_HashMap(8);
    map->put(map, "key1", 10);
    map->put(map, "somethingelse", 2);
    map->put(map, "anotherone", 1);
    map->put(map, "whatuupp", 1000);
    map->dump(map);
    printf("map[\"%s\"] == %d\n", "key1", map->get(map, "key1", -1));
    printf("map[\"%s\"] == %d\n", "anotherone", map->get(map, "anotherone", -1));
    printf("map[\"%s\"] == %d\n", "daaang", map->get(map, "daaang", -1));
}