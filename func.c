#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "Stuct.h"

void gen_full_name(char* full_name)
{
    const char* first[] = { "James", "Mary", "John", "Patricia", "Robert",
        "Jennifer", "Michael", "Linda", "William", "Elizabeth",
        "David", "Barbara", "Richard", "Susan", "Joseph",
        "Jessica", "Thomas", "Sarah", "Charles", "Karen",
        "Christopher", "Nancy", "Daniel", "Lisa", "Matthew",
        "Margaret", "Anthony", "Betty", "Mark", "Dorothy" };
    const char* last[] = { "Smith", "Johnson", "Williams", "Brown", "Jones",
        "Garcia", "Miller", "Davis", "Rodriguez", "Martinez",
        "Hernandez", "Lopez", "Gonzalez", "Wilson", "Anderson",
        "Thomas", "Taylor", "Moore", "Jackson", "Martin",
        "Lee", "Perez", "Thompson", "White", "Harris",
        "Sanchez", "Clark", "Ramirez", "Lewis", "Robinson" };

    int first_index = rand() % 30;
    int last_index = rand() % 30;
    sprintf(full_name, "%s %s", first[first_index], last[last_index]);
}

void gen_region(char* region)
{
    const char* regs[] = {
        "Moskva", "Sankt-Peterburg", "Krasnodarskiy kray", "Tatarstan", "Sverdlovskaya oblast",
        "Bashkortostan", "Rostovskaya oblast", "Chelyabinskaya oblast", "Permskiy kray", "Krasnoyarskiy kray",
        "Samarskaya oblast", "Nizhegorodskaya oblast", "Tatarstan", "Novosibirskaya oblast", "Dagestan",
        "Stavropolskiy kray", "Kemerovskaya oblast", "Irkutskaya oblast", "Leningradskaya oblast", "Moskovskaya oblast",
        "Tverskaya oblast", "Saratovskaya oblast", "Altayskiy kray", "Voronezhskaya oblast", "Omskaya oblast",
        "Khabarovskiy kray", "Primorskiy kray", "Tulskaya oblast", "Yaroslavskaya oblast", "Kareliya"
    };

    int index = rand() % 30;
    strcpy(region, regs[index]);
}

void gen_subject(char* subject)
{
    const char* subjects[] = { "Mathematics", "Physics", "Chemistry", "Biology", "Geography",
        "History", "Social Studies", "Literature", "English", "Foreign Language",
        "Art", "Music", "Physical Education", "Computer Science", "Technology",
        "Economics", "Law", "Psychology", "Algebra", "Geometry",
        "Calculus", "Environmental Science", "World History", "National Language", "Health",
        "Astronomy", "Drama", "Robotics", "Political Science", "Statistics" };

    int index = rand() % 30;
    strcpy(subject, subjects[index]);
}

double get_time_diff(clock_t start, clock_t end)
{
    double diff = (double)(end - start) / CLOCKS_PER_SEC;
    if (diff < 1e-9) {
        diff = 1e-9; 
    }
    return diff;
}

void fill_struct_with_random(Winner* w)
{
    gen_full_name(w->full_name);
    w->school_number = rand() % 1000 + 1;
    gen_region(w->region);
    gen_subject(w->subject);
    w->year = rand() % 50 + 1980;
}

const char* funcs[3] = { "hash1", "hash2", "hash3" };

hash_size hash1(const char* string)
{
    hash_size hash = 0;
    char c;

    while (c = *string++)
    {
        hash = (hash * 131) + c;
    }

    return hash;
}

hash_size hash2(const char* string)
{
    hash_size hash = 0;
    char c;

    while (c = *string++)
    {
        hash = c + (hash << 6) + (hash << 16) - hash;
    }

    return hash;
}

hash_size hash3(const char* string)
{
    hash_size hash = 0;
    char c;

    while (c = *string++)
    {
        hash += c;
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }

    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);

    return hash;
}

hash_f get_hash_func(const long option) {
    if (option == 0) {
        return hash1;
    }
    if (option == 1) {
        return hash2;
    }
    if (option == 2) {
        return hash3;
    }
    fprintf(stderr, "Unrecognized option %ld!\n", option);
    exit(1);
}

hashtable* Create_table(long capacity, hash_f hash_func)
{
    hashtable* table = malloc(sizeof(hashtable));
    if (!table) return NULL;

    table->capacity = capacity;
    table->hash_func = hash_func;
    table->collisions = 0;
    table->size = 0;
    table->entries = calloc(table->capacity, sizeof(Node*));

    if (!table->entries) {
        free(table);
        return NULL;
    }

    return table;
}

void Delete_table(hashtable* table)
{
    if (!table) return;

    for (long i = 0; i < table->capacity; i++)
    {
        Node* element = table->entries[i];
        while (element != NULL)
        {
            Node* next = element->next;
            if (element->key)
            {
                free(element->key);
            }
            free(element);
            element = next;
        }
    }

    free(table->entries);
    free(table);
}

Node* get_entry(hashtable* table, long index, char* key)
{
    Node* element = table->entries[index];
    while (element != NULL)
    {
        if (strcmp(element->key, key) == 0)
        {
            return element;
        }
        element = element->next;
    }

    return NULL;
}

void add_entry(hashtable* table, char* key, void* value)
{
    if (!table || !key) return;

    long index = table->hash_func(key) % table->capacity;
    Node* current = table->entries[index];

    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            return; 
        }
        current = current->next;
    }

    if (table->entries[index] != NULL) {
        table->collisions++;
    }

    Node* new_node = malloc(sizeof(Node));
    if (!new_node) return;

    new_node->key = key;
    new_node->value = value;
    new_node->next = table->entries[index];
    table->entries[index] = new_node;

    table->size++;
}

void* get_value(hashtable* table, char* key)
{
    if (!table || !key) return NULL;

    long index = table->hash_func(key) % table->capacity;
    const Node* entry = get_entry(table, index, key);

    return entry ? entry->value : NULL;
}

int search_all(hashtable* table, const char* key)
{
    if (!table || !key) return 0;

    long index = table->hash_func(key) % table->capacity;
    Node* current = table->entries[index];
    int found = 0;

    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            found++;
        }
        current = current->next;
    }

    return found;
}