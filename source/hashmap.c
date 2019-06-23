#include "hashmap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct hashmap_element {
  char* key;
  char* value;
  bool in_use;
} hashmap_element;
typedef hashmap_element* hashmap_element_t;

typedef struct hashmap {
  size_t max_size;
  size_t current_size;
  hashmap_element_t data;
} hashmap;

#define DEFAULT_MAX_SIZE (256)
#define MAX_CHAIN_LENGTH (8)
#define HASHMAP_FULL (-1)

#define LOAD_FACTOR (0.75)
// BKDR Hash Function
static size_t BKDRHash(const char* str) {
  unsigned int seed = 131;
  unsigned int hash = 0;

  while (*str) {
    hash = hash * seed + (*str++);
  }

  return (hash & 0x7FFFFFFF);
}

static ssize_t hashmap_get_available_index(hashmap_t map, const char* key) {
  if (map->current_size >= map->max_size * LOAD_FACTOR) {
    return HASHMAP_FULL;
  }
  unsigned int index = BKDRHash(key) % map->max_size;
  hashmap_element_t element = NULL;
  for (size_t i = 0; i < MAX_CHAIN_LENGTH; i++) {
    element = &map->data[index];
    if (!element->in_use) {
      return index;
    }

    if (element->in_use && strcmp(element->key, key) == 0) {
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

  element->in_use = false;
}
static void hashmap_free_data(hashmap_element_t data, size_t max_size) {
  hashmap_element_t element = NULL;
  for (size_t i = 0; i < max_size; i++) {
    element = &data[i];
    hashmap_clear_element(element);
  }
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

static ssize_t hashmap_rehash(hashmap_t map) {
  size_t new_max_size = 2 * map->max_size;
  hashmap_element_t new_data =
      (hashmap_element_t)malloc(new_max_size * sizeof(hashmap_element));
  if (!new_data) {
    return HASHMAP_MALLOC_FAIL;
  }

  size_t old_max_size = map->max_size;
  map->max_size = new_max_size;
  hashmap_element_t old_data = map->data;
  map->data = new_data;
  map->current_size = 0;

  hashmap_element_t element = NULL;
  for (size_t i = 0; i < old_max_size; i++) {
    element = &old_data[i];
    if (element->in_use) {
      ssize_t status = hashmap_put(map, element->key, element->value);
      if (status != HASHMAP_OK) {
        hashmap_free_data(old_data, old_max_size);
        return status;
      }
    }
  }
  hashmap_free_data(old_data, old_max_size);
  return HASHMAP_OK;
}

hashmap_t hashmap_new() { return hashmap_new_with_size(DEFAULT_MAX_SIZE); }

void hashmap_free(hashmap_t map) {
  hashmap_free_data(map->data, map->max_size);
  free(map->data);
  free(map);
}

ssize_t hashmap_put(hashmap_t map, char* key, char* value) {
  ssize_t index = hashmap_get_available_index(map, key);
  ssize_t status = HASHMAP_OK;
  while (index == HASHMAP_FULL) {
    if ((status = hashmap_rehash(map)) != HASHMAP_OK) {
      return status;
    }
    index = hashmap_get_available_index(map, key);
  }

  if (map->data[index].in_use) {
    free(map->data[index].key);
    free(map->data[index].value);
  } else {
    map->data[index].in_use = true;
    map->current_size++;
  }
  map->data[index].key = strdup(key);
  map->data[index].value = strdup(value);

  return HASHMAP_OK;
}
ssize_t hashmap_get(hashmap_t map, char* key, char** value) {
  unsigned int index = BKDRHash(key) % map->max_size;
  hashmap_element_t element = NULL;
  for (size_t i = 0; i < MAX_CHAIN_LENGTH; i++) {
    element = &map->data[index];
    if (element->in_use && strcmp(element->key, key) == 0) {
      *value = element->value;
      return HASHMAP_OK;
    }

    index = (index + 1) % map->max_size;
  }

  return HASHMAP_NO_SUCH_ELEMENT;
}
ssize_t hashmap_remove(hashmap_t map, char* key) {
  unsigned int index = BKDRHash(key) % map->max_size;
  hashmap_element_t element = NULL;
  for (size_t i = 0; i < MAX_CHAIN_LENGTH; i++) {
    element = &map->data[index];
    if (element->in_use && strcmp(element->key, key) == 0) {
      hashmap_clear_element(element);
      map->current_size--;
      return HASHMAP_OK;
    }

    index = (index + 1) % map->max_size;
  }
  return HASHMAP_NO_SUCH_ELEMENT;
}
size_t hashmap_get_size(hashmap_t map) {
  if (map) {
    return map->current_size;
  }
  return 0;
}
ssize_t hashmap_iterate(hashmap_t map, IterateCallback iterateCallback) {
  hashmap_element_t element = NULL;
  ssize_t status = HASHMAP_OK;
  for (size_t i = 0; i < map->max_size; i++) {
    element = &map->data[i];
    if (element->in_use) {
      status = iterateCallback(element->key, element->value);
      if (status != HASHMAP_OK) {
        return status;
      }
    }
  }

  return HASHMAP_OK;
}