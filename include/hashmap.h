#ifndef __HASHMAP_H__
#define __HASHMAP_H__
#ifdef __cplusplus
extern "C" {
#endif  //__cplusplus

#include <sys/types.h>
#include <stdbool.h>

#define HASHMAP_OK (0)
#define HASHMAP_NO_SUCH_ELEMENT (-1)
#define HASHMAP_MALLOC_FAIL (-2)
#define HASHMAP_EMPTY (-3)


typedef ssize_t (*IterateCallback)(const char *key, const char *value);

extern void* hashmap_new();
extern void hashmap_free(void* in);
extern ssize_t hashmap_put(void* in, char* key, char* value);
extern ssize_t hashmap_get(void* in, char* key, char** value);
extern ssize_t hashmap_remove(void* in, char* key);
extern size_t hashmap_get_size(void* in);
extern ssize_t hashmap_iterate(void* in, IterateCallback iterateCallback);

#ifdef __cplusplus
}
#endif  //__cplusplus
#endif  //__HASHMAP_H__