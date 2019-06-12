#include "hashmap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TABLE_SIZE (256)

// BKDR Hash Function
static unsigned int BKDRHash(const char* str) {
    unsigned int seed = 131;
    unsigned int hash = 0;

    while (*str) {
        hash = hash * seed + (*str++);
    }

    return (hash & 0x7FFFFFFF);
}
static unsigned int hashmap_hash(const char* string, unsigned int max_size) {
    return BKDRHash(string) % max_size;
}

hashmap_t hashmap_new() {
    hashmap_t map = (hashmap_t)malloc(sizeof(hashmap));
    if (!map) goto err;

    map->data = (hashmap_element_t)malloc(TABLE_SIZE * sizeof(hashmap_element));
    if (!map->data) goto err;

    map->max_size = TABLE_SIZE;
    map->current_size = 0;

    return map;
err:
    if (map) hashmap_free(map);
    return NULL;
}
void hashmap_free(hashmap_t map) {
    if (map && map->data) {
        free(map->data);
    }

    if (map) {
        free(map);
    }
}
int hashmap_put(hashmap_t map, char* key, char* value) {
    unsigned int index = hashmap_hash(key, map->max_size);
    hashmap_element_t head = &map->data[index], pre;
    while (head) {
        if (head->in_use == 0) {
            head->in_use = 1;
            head->key = key;
            head->value = value;
            map->current_size++;
            return HASHMAP_OK;
        } else if (head->in_use == 1 && strcmp(key, head->key) == 0) {
            if (head->value) {
                free(head->value);
            }
            head->value = value;
            map->current_size++;
            return HASHMAP_OK;
        }
        pre = head;
        head = head->next;
    }
    head = (hashmap_element_t)malloc(sizeof(hashmap_element));
    if (!head) {
        return HASHMAP_MALLOC_FAIL;
    }
    memset(head, 0, sizeof(hashmap_element));
    head->in_use = 1;
    head->key = key;
    head->value = value;
    pre->next = head;
    map->current_size++;
    return HASHMAP_OK;
}
int hashmap_get(hashmap_t map, char* key, char** value) {
    unsigned int index = hashmap_hash(key, map->max_size);
    hashmap_element_t head = &map->data[index];
    while (head) {
        if (head->in_use == 1 && strcmp(key, head->key) == 0) {
            *value = head->value;
            return HASHMAP_OK;
        }
        head = head->next;
    }
    return HASHMAP_NO_SUCH_ELEMENT;
}
int hashmap_remove(hashmap_t map, char* key) {
    unsigned int index = hashmap_hash(key, map->max_size);
    hashmap_element_t head = &map->data[index], pre;
    while (head) {
        pre = head;
        if (head->in_use == 1 && strcmp(key, head->key) == 0) {
            pre->next = head->next;
            if (head->key) {
                free(head->key);
            }

            if (head->value) {
                free(head->value);
            }

            free(head);
            map->current_size--;
            return HASHMAP_OK;
        }
        head = head->next;
    }
    return HASHMAP_NO_SUCH_ELEMENT;
}
int hashmap_get_size(hashmap_t map) {
    if (map) {
        return map->current_size;
    }
    return 0;
}
int hashmap_iterate(hashmap_t map, IterateCallback iterateCallback) {
    int i, status = HASHMAP_OK;
    hashmap_element_t head = NULL;
    if (map->current_size <= 0) {
        return HASHMAP_EMPTY;
    }
    for (i = 0; i < map->max_size; i++) {
        head = &map->data[i];
        while (head) {
            if (head->in_use == 1) {
                status = iterateCallback(head);
                if (status != HASHMAP_OK) {
                    return status;
                }
            }
            break;
        }
    }
    return HASHMAP_OK;
}