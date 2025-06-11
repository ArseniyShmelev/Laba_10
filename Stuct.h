#pragma once
#include <stdio.h>
#include <time.h>

#define MAX_NAME 50
#define MAX_REGION 50
#define MAX_SUBJECT 50
#define MAX_LENGTH 50
#define CONST 25

typedef unsigned long long hash_size;
typedef hash_size(*hash_f)(const char*);

typedef struct {
	char full_name[MAX_NAME];
	int school_number;
	char region[MAX_REGION];
	char subject[MAX_SUBJECT];
	int year;
} Winner;

typedef struct Node
{
	char* key;
	void* value;
	struct Node* next;
} Node;

typedef struct hashtable
{
	Node** entries;
	hash_f hash_func;
	long capacity;
	long size;
	long collisions;
} hashtable;


hash_size hash1(const char* string);
hash_size hash2(const char* string);
hash_size hash3(const char* string);

extern const char* funcs[3];

void gen_full_name(char* full_name);
void gen_region(char* region);
void gen_subject(char* subject);
void fill_struct_with_random(Winner* w);
double get_time_diff(clock_t start, clock_t end);
hashtable* Create_table(long capacity, hash_f hash_func);
void Delete_table(hashtable* table);
void add_entry(hashtable* table, char* key, void* value);
void* get_value(hashtable* table, char* key);  
Node* get_entry(hashtable* table, long index, char* key);
hash_f get_hash_func(long option);
int search_all(hashtable* table, const char* key);