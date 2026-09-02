/*
 * employee_id_double_hashing.c
 *
 * Hash table implementation for storing and looking up employee IDs
 * using open addressing with double hashing for collision resolution.
 *
 * Compile:   gcc -o employee_id_double_hashing employee_id_double_hashing.c
 * Run:       ./employee_id_double_hashing
 *
 * Expected Output:
 *   Employee exists: Yes
 */

#include <stdio.h>
#include <stdlib.h>

#define TABLE_SIZE 10007
#define LOAD_LIMIT 0.70

typedef struct Node {
    long employeeId;
    struct Node *next;
} Node;

typedef struct {
    long slots[TABLE_SIZE];
    int occupied[TABLE_SIZE];
    int count;
} DoubleHashTable;

/* Primary hash function */
unsigned int hash1(long employeeId) {
    return (unsigned int)(employeeId % TABLE_SIZE);
}

/* Secondary hash function used as the probing step size.
 * TABLE_SIZE is prime, so (TABLE_SIZE - 1) guarantees a step
 * that is coprime with TABLE_SIZE for all non-zero values,
 * ensuring every slot can be probed. */
unsigned int hash2(long employeeId) {
    return 1 + (unsigned int)(employeeId % (TABLE_SIZE - 1));
}

/* Returns 1 if employeeId is present in the table, 0 otherwise. */
int contains(DoubleHashTable *table, long employeeId) {
    unsigned int start = hash1(employeeId);
    unsigned int step = hash2(employeeId);

    for (int i = 0; i < TABLE_SIZE; i++) {
        unsigned int pos = (start + i * step) % TABLE_SIZE;

        if (!table->occupied[pos])
            return 0;

        if (table->slots[pos] == employeeId)
            return 1;
    }

    return 0;
}

/* Inserts employeeId into the table.
 * Returns:
 *   1  on successful insertion
 *   0  if the employeeId already exists
 *  -1  if the table is full or the load limit has been reached
 */
int insert(DoubleHashTable *table, long employeeId) {
    unsigned int start = hash1(employeeId);
    unsigned int step = hash2(employeeId);

    if ((float)table->count / TABLE_SIZE >= LOAD_LIMIT)
        return -1;

    for (int i = 0; i < TABLE_SIZE; i++) {
        unsigned int pos = (start + i * step) % TABLE_SIZE;

        if (!table->occupied[pos]) {
            table->slots[pos] = employeeId;
            table->occupied[pos] = 1;
            table->count++;
            return 1;
        }

        if (table->slots[pos] == employeeId)
            return 0;
    }

    return -1;
}

int main(void) {
    DoubleHashTable employees = {0};

    insert(&employees, 192524440L);

    printf(
        "Employee exists: %s\n",
        contains(&employees, 192524440L) ? "Yes" : "No"
    );

    return 0;
}
