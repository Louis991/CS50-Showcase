// Implements a dictionary's functionality

#include <stdbool.h>
#include "dictionary.h"
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 65536;

// Hash table
node *table[N];

// Variable to count the number of words loaded.
unsigned int wordCount = 0;

// State of the dictionary, loaded = true, unloaded = false.
bool state = false;

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // Gets the key corresponding to the word.
    unsigned int key = hash(word);

    // Traverse through the linked list.
    for (node *traverse = table[key]; traverse != NULL; traverse = traverse->next)
    {
        // If there's a match.
        if (strcasecmp(traverse->word, word) == 0)
        {
            return true;
        }
    }

    // If the word wasn't found in the dictionary.
    return false;
}

// Hashes word to a number.
// djb2 hash, written by Daniel J. Bernstein. Edited to work within the limitations of an unsigned int return value and a strictly lowercase word.
unsigned int hash(const char *word)
{
    unsigned long hash = 5381;
    int c;

    while ((c = *word++))
    {
        hash = ((hash << 5) + hash) + tolower(c); /* hash * 33 + c */
    }
    unsigned int key = hash % 65535;
    return key;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Open dictionary stream
    FILE *dict = fopen(dictionary, "r");

    // If it can't be opened successfully, end.
    if (dict == NULL)
    {
        return false;
    }

    // Temporary buffer overwritten for each word.
    char buffer[LENGTH + 1];

    // Scan through the whole dictionary, until the end of file.
    while (fscanf(dict, "%s", buffer) != EOF)
    {
        // Create a new node.
        node *n = malloc(sizeof(node));
        // If memory for the pointer couldn't be allocated, quit.
        if (n == NULL)
        {
            return false;
        }

        // Fill node information.
        strcpy(n->word, buffer);
        n->next = NULL;

        // Get the key corresponding to the current word.
        unsigned int key = hash(buffer);

        // Increment the dictionary word count.
        wordCount++;

        // If there's no previous entry, append by editing the linked list's head.
        if (table[key] == NULL)
        {
            table[key] = n;
        }
        // If there's a previous entry, append the node after the linked list's head
        else
        {
            n->next = table[key]->next;
            table[key]->next = n;
        }
    }

    // Close dictionary stream, free dynamically allocated memory and quit, returning true.
    fclose(dict);
    state = true;
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    if (state == true)
    {
        return wordCount;
    }
    else
    {
        return 0;
    }
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // Go through each linked list in table[N].
    for (int i = 0; i < N; i++)
    {
        if (table[i] != NULL)
        {
            node *traverse = table[i];
            while (traverse != NULL)
            {
                node *temporal = traverse;
                traverse = traverse->next;
                free(temporal);
            }
        }
    }
    state = false;
    return true;
}

