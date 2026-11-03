#include "utils/hashmap.h"
#include <strings.h>
#include <stdlib.h>

HashMap* createHashmap(){
    HashMap* hashmap = calloc(1, sizeof(HashMap));
    return hashmap;
}

uint64_t hashString(const char *str)
{
    uint64_t hash = 14695981039346656037ULL;

    while (*str)
    {
        hash ^= (unsigned char)*str++;
        hash *= 1099511628211ULL;
    }

    return hash;
}

void insertIntoHashmap(HashMap* hashmap, char* key, char* value){
    int index = hashString(key) % HASHMAP_SIZE;
    HashEntry* entry = calloc(1, sizeof(HashEntry));
    entry->key = key;
    entry->value = value;
    
    entry->next = hashmap->entries[index];
    hashmap->entries[index] = entry;
    
}

char* getFromHashmap(HashMap* hashmap, char* key){
    int index = hashString(key) % HASHMAP_SIZE;
    HashEntry* entry = hashmap->entries[index];
    while (entry)
    {
        if(!strcasecmp(entry->key, key)){
            return entry->value;
        }
        entry = entry->next;
    }

    return NULL;
}