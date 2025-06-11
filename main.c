#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "Stuct.h"

const long sizes[] = { 1000, 10000, 50000, 100000, 200000, 500000, 1000000 };

int main(void)
{
    srand((unsigned int)time(NULL));

    printf("Hash Table Performance Test\n");
    printf("===========================\n");
    printf("Testing hash functions with different table sizes\n");
    printf("Each time measurement is average of 1,000,000 searches\n\n");

    FILE* collisions_file = fopen("collision.csv", "w");
    FILE* time_file = fopen("time.csv", "w");

    if (!collisions_file || !time_file) {
        printf("Failed to open output files.\n");
        return 1;
    }

    fprintf(collisions_file, "Size,Hash1_Collisions,Hash2_Collisions,Hash3_Collisions\n");
    fprintf(time_file, "Size,Hash1_Time_sec,Hash2_Time_sec,Hash3_Time_sec\n");

    for (int s = 0; s < 7; ++s) {
        long count = sizes[s];
        printf("\n--- Testing size: %ld ---\n", count);

        long collisions[3] = { 0 };
        double times[3] = { 0.0 };

        for (int f = 0; f < 3; ++f) {
            hash_f hf = get_hash_func(f);
            hashtable* table = Create_table(count * 2, hf);

            if (!table) {
                printf("Failed to create hash table\n");
                continue;
            }

            Winner** data = malloc(sizeof(Winner*) * count);
            if (!data) {
                printf("Failed to allocate memory for data\n");
                Delete_table(table);
                continue;
            }

            for (long i = 0; i < count; ++i) {
                data[i] = malloc(sizeof(Winner));
                if (!data[i]) {
                    printf("Failed to allocate memory for Winner\n");
                    for (long j = 0; j < i; ++j) {
                        free(data[j]);
                    }
                    free(data);
                    Delete_table(table);
                    goto next_iteration;
                }

                fill_struct_with_random(data[i]);
                char* key = strdup(data[i]->full_name);
                if (!key) {
                    printf("Failed to duplicate string\n");
                    free(data[i]);
                    continue;
                }
                add_entry(table, key, data[i]);
            }

            
            char* target = data[rand() % count]->full_name;

            
            const int num_searches = 1000000; 
            clock_t start = clock();
            for (int i = 0; i < num_searches; i++) {
                search_all(table, target);
            }
            clock_t end = clock();
            double t = get_time_diff(start, end) / num_searches; 

            
            collisions[f] = table->collisions;
            times[f] = t;

            printf("Hash%d | Collisions: %ld | Time: %.9f sec | Fill Factor: %.2f\n",
                f + 1, table->collisions, t, (double)table->size / table->capacity);

            for (long i = 0; i < count; ++i) {
                free(data[i]);
            }
            free(data);
            Delete_table(table);

        next_iteration:;
        }

        
        fprintf(collisions_file, "%ld,%ld,%ld,%ld\n", count, collisions[0], collisions[1], collisions[2]);
        fprintf(time_file, "%ld,%.9f,%.9f,%.9f\n", count, times[0], times[1], times[2]);
    }

    fclose(collisions_file);
    fclose(time_file);

    printf("\nTest complete. Results saved to:\n");
    printf("- collision.csv (in current working directory)\n");
    printf("- time.csv (in current working directory)\n");

    return 0;
}