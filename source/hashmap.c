#include "hashmap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define DEFAULT_MAX_SIZE (256)
#define MAX_CHAIN_LENGTH (8)
#define HASHMAP_FULL (-1)

#define LOAD_FACTOR (0.75)
// BKDR Hash Function
static unsigned int BKDRHash(const char* str) {
    unsigned int seed = 131;
    unsigned int hash = 0;

    while (*str) {
        hash = hash * seed + (*str++);
    }

    return (hash & 0x7FFFFFFF);
}

static int hashmap_get_available_index(hashmap_t map, const char* key) {
    unsigned int index = BKDRHash(key) % map->max_size;
    hashmap_element_t element = NULL;
    for (size_t i = 0; i < MAX_CHAIN_LENGTH; i++) {
        element = &map->data[index];
        if (element->in_use == 0) {
            return index;
        }

        if ((element->in_use == 1) && strcmp(element->key, key) == 0) {
            return index;
        }
        index = (index + 1) % map->max_size;
    }

    return HASHMAP_FULL;
}

static void hashmap_clear_element(hashmap_element_t element) {
    if (element->key) {
        free(element->key);
    }

    if (element->value) {
        free(element->value);
    }

    element->in_use = 0;
}

static hashmap_t hashmap_new_with_size(unsigned int max_size) {
    hashmap_t map = (hashmap_t)malloc(sizeof(hashmap));
    if (!map) goto err;

    map->data = (hashmap_element_t)malloc(max_size * sizeof(hashmap_element));
    if (!map->data) goto err;

    map->max_size = max_size;
    map->current_size = 0;

    return map;
err:
    if (map) hashmap_free(map);
    return NULL;
}

static int hashmap_rehash(hashmap_t* map) {
    unsigned int new_max_size = 2 * (*map)->max_size;
    hashmap_t new_map = hashmap_new_with_size(new_max_size);
    if (!new_map) {
        return HASHMAP_MALLOC_FAIL;
    }

    hashmap_element_t element = NULL;
    for (size_t i = 0; i < (*map)->max_size; i++) {
        element = &(*map)->data[i];
        if (element->in_use == 1) {
            int status = hashmap_put(new_map, element->key, element->value);
            if (status != HASHMAP_OK) {
                hashmap_free(new_map);
                return status;
            }
        }
    }

    hashmap_free(*map);
    *map = new_map;

    return HASHMAP_OK;
}

hashmap_t hashmap_new() { return hashmap_new_with_size(DEFAULT_MAX_SIZE); }

void hashmap_free(hashmap_t map) {
    hashmap_element_t element = NULL;
    for (size_t i = 0; i < map->max_size; i++) {
        element = &map->data[i];
        hashmap_clear_element(element);
    }

    free(map->data);
    free(map);
}

int hashmap_put(hashmap_t map, char* key, char* value) {
    int index = hashmap_get_available_index(map, key);
    int status = HASHMAP_OK;
    while (index == HASHMAP_FULL) {
        if ((status = hashmap_rehash(&map)) != HASHMAP_OK) {
            return status;
        }
        index = hashmap_get_available_index(map, key);
    }

    map->data[index].in_use = 1;
    map->data[index].key = strdup(key);
    map->data[index].value = strdup(value);
    map->current_size++;

    return HASHMAP_OK;
}
int hashmap_get(hashmap_t map, char* key, char** value) {
    unsigned int index = BKDRHash(key) % map->max_size;
    hashmap_element_t element = NULL;
    for (size_t i = 0; i < MAX_CHAIN_LENGTH; i++) {
        element = &map->data[index];
        if (element->in_use == 1 && strcmp(element->key, key) == 0) {
            *value = map->data[i].value;
            return HASHMAP_OK;
        }

        index = (index + 1) % map->max_size;
    }

    return HASHMAP_NO_SUCH_ELEMENT;
}
int hashmap_remove(hashmap_t map, char* key) {
    unsigned int index = BKDRHash(key) % map->max_size;
    hashmap_element_t element = NULL;
    for (size_t i = 0; i < MAX_CHAIN_LENGTH; i++) {
        element = &map->data[index];
        if (element->in_use == 1 && strcmp(element->key, key) == 0) {
            hashmap_clear_element(element);
            return HASHMAP_OK;
        }

        index = (index + 1) % map->max_size;
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
    hashmap_element_t element = NULL;
    int status = HASHMAP_OK;
    for (size_t i = 0; i < map->max_size; i++) {
        element = &map->data[i];
        if (element->in_use == 1) {
            status = iterateCallback(element);
            if (status != HASHMAP_OK) {
                return status;
            }
        }
    }

    return HASHMAP_OK;
}