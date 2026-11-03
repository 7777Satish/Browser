#ifndef HASHMAP_H
#define HASHMAP_H

#define HASHMAP_SIZE 200

#include <stdio.h>
#include <stdint.h>

typedef struct HashEntry
{
    char *key;
    char *value;
    struct HashEntry *next;
} HashEntry;

typedef struct
{
    HashEntry *entries[HASHMAP_SIZE];
} HashMap;

HashMap* createHashmap();
uint64_t hashString(const char *str);
void insertIntoHashmap(HashMap* hashmap, char* key, char* value);
char* getFromHashmap(HashMap* hashmap, char* key);

#endif