// Problem Set 5
// Angus Leung
// May 3, 2020

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <stdint.h>

// Implements a dictionary's functionality

#include <stdbool.h>

#include "dictionary.h"

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

// Word Counter for Size, used in Load
unsigned int wordcount = 0;

// Loaded flag if load is complete, for size
bool loaded = false;

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // Dictionary is already lowercase. Change input word to lowercase
    int wordlength = strlen(word);
    char *in_word = malloc(wordlength + 1);
    
    for (int i = 0; i < wordlength + 1; i++)
    {
        in_word[i] = tolower(word[i]);
    }

    
    node *cursor = table[hash(in_word)];
    
    while (cursor != NULL)
    {
        if (strcmp(in_word, cursor->word) == 0)
        {
            free(in_word);
            return true;
        }
        else
        {
            cursor = cursor->next;
        }
    }
    free(in_word);
    return false;
}

// Hashes word to a number

unsigned int hash(const char *word)
{
    // https://www.reddit.com/r/cs50/comments/1x6vc8/pset6_trie_vs_hashtable/cf9nlkn/
    // unsigned long hash = 65536;
    // int c = *word;
    // c = tolower(c);
    // while (*word != 0)
    // {
    //     hash = ((hash << 5) + hash) + c;
    //     c = *word++;
    //     c = tolower(c);
    // }
    // return hash % N;
    
    
    // https://github.com/curiouskiwi/cs50x-help/blob/master/hash.c
    uint32_t hash = 0;
    while (*word)
    {
        hash = (hash << 2) ^ *word; //(*s | 0x20);
        word++;
    }

    // return a value between 0 and 65535
    return (int)((hash >> 16) ^ (hash & 0xffff));
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    FILE *in_file = fopen(dictionary, "r");
    char *in_word = malloc(LENGTH + 1);
    int index = 0;
    
    for (int i = 0; i < N; i++)
    {
        table[i] = NULL;
    }
    
    if (in_file == NULL)
    {
        printf("File cannot be opened\n");
        return false;
    }
    // printf("%i\n %i \n", hash("A"), hash("a"));
    while (fscanf(in_file, "%s", in_word) != EOF)
    {
        wordcount++;
        node *new_node = malloc(sizeof(node));
        if (new_node == NULL)
        {
            unload();
            return false;
        }
        else
        {
            strcpy(new_node->word, in_word);
            index = hash(new_node->word);
            
            // Makes new_node the new head if head does not exist
            if (table[index] == NULL)
            {
                table[index] = new_node;
                table[index]->next = NULL;
            }
            else
            {
                new_node->next = table[index];
                table[index] = new_node;
            }
        }

    }
    fclose(in_file);
    free(in_word);
    loaded = true;
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    if (loaded == true)
    {
        return wordcount;
    }
    else
    {
        return 0;
    }
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    node *cursor = NULL;
    node *tmp = NULL;
    
    for (int i = 0; i < N; i++)
    {
        cursor = table[i];
        while (cursor != NULL)
        {
            tmp = cursor;
            cursor = cursor->next;
            free(tmp);
        }
        free(cursor);
    }

    return true;
}
