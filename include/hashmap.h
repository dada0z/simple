#ifndef __HASHMAP_H__
#define __HASHMAP_H__
#ifdef __cplusplus
extern "C" {
#endif  //__cplusplus

#include <stdbool.h>
#include <sys/types.h>

#define HASHMAP_OK (0)
#define HASHMAP_NO_SUCH_ELEMENT (-1)
#define HASHMAP_MALLOC_FAIL (-2)
#define HASHMAP_EMPTY (-3)

typedef struct hashmap* hashmap_t;
typedef ssize_t (*IterateCallback)(const char* key, const char* value);

extern hashmap_t hashmap_new();
extern void hashmap_free(hashmap_t map);
extern ssize_t hashmap_put(hashmap_t *map, char* key, char* value);
extern ssize_t hashmap_get(hashmap_t map, char* key, char** value);
extern ssize_t hashmap_remove(hashmap_t map, char* key);
extern size_t hashmap_get_size(hashmap_t map);
extern ssize_t hashmap_iterate(hashmap_t map, IterateCallback iterateCallback);

#ifdef __cplusplus
}
#endif  //__cplusplus
#endif  //__HASHMAP_H__