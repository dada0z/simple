#ifndef __HASHMAP_H__
#define __HASHMAP_H__

#if defined(_MSC_VER)
#include <BaseTsd.h>
typedef SSIZE_T ssize_t;
#else
#include <sys/types.h>
#endif

#define HASHMAP_OK (0)
#define HASHMAP_NO_SUCH_ELEMENT (-1)
#define HASHMAP_MALLOC_FAIL (-2)
#define HASHMAP_EMPTY (-3)

typedef struct hashmap_element {
    char* key;
    char* value;
    size_t in_use;
} hashmap_element;
typedef hashmap_element* hashmap_element_t;

typedef struct hashmap {
    size_t max_size;
    size_t current_size;
    hashmap_element_t data;
} hashmap;

typedef hashmap* hashmap_t;
typedef ssize_t (*IterateCallback)(const hashmap_element_t element);

extern hashmap_t hashmap_new();
extern void hashmap_free(hashmap_t map);
extern ssize_t hashmap_put(hashmap_t map, char* key, char* value);
extern ssize_t hashmap_get(hashmap_t map, char* key, char** value);
extern ssize_t hashmap_remove(hashmap_t map, char* key);
extern size_t hashmap_get_size(hashmap_t map);
extern ssize_t hashmap_iterate(hashmap_t map, IterateCallback iterateCallback);

#endif  //__HASHMAP_H__