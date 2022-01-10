#include "..\DoublyLinkedLists.\ToIncludeInSets.\DoublyLinkedList.h"
#include <stddef.h> //required for NULL
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

size_t djb33x_hash(const char *key, const size_t keylen)
{
    size_t hash = 5381;
    for (size_t i = 0; i < keylen; i++)
    {
        hash = ((hash << 5) + hash) ^ key[i];
    }
    return hash;
}

#define KeyFromNode(node) ((key_value_t *)(node->data))

typedef struct key_value
{
    const char *key;
    size_t key_len;
} key_value_t;

// typedef struct set_table
// {
//     struct set_node **nodes;
//     size_t hashmap_size;
// } set_table_t;

// typedef struct set_node
// {
//     const char *key;
//     size_t key_len;
//     struct set_node *next;
//     struct set_node *prev;
// } set_node_t;

typedef struct set_table
{
    list_node_t **nodes;
    size_t hashmap_size;
    void *data_to_key;
} set_table_t;

const char *data_to_key(list_node_t *node)
{
    return KeyFromNode(node)->key;
}

struct set_table *set_table_new(const size_t hashmap_size, const char *data_to_key(list_node_t *))
{
    struct set_table *table = malloc(sizeof(set_table_t));
    if (!table)
    {
        return NULL;
    }
    table->hashmap_size = hashmap_size;
    table->nodes = calloc(table->hashmap_size, sizeof(list_node_t *));
    if (!table->nodes)
    {
        free(table);
        return NULL;
    }
    table->data_to_key = data_to_key;
    return table;
}

list_node_t *set_search(set_table_t *table, const char *key, const size_t key_len)
{
    size_t hash = djb33x_hash(key, key_len);
    size_t index = hash % table->hashmap_size;
    list_node_t *node = table->nodes[index];
    while (node)
    {
        if (KeyFromNode(node)->key_len && KeyFromNode(node)->key == key)
        {
            return node;
        }
        node = node->next;
    }
    return NULL;
}

// set_node_t *set_search(set_table_t *table, const char *key)
// {
//     if (!table)
//         return NULL;
//     size_t index = 0;
//     set_node_t *node_temp = table->nodes[index];
//     for (size_t i = 0; i < table->hashmap_size; i++)
//     {
//         while (node_temp)
//         {
//             if (node_temp->key_len && node_temp->key == key)
//             {
//                 return node_temp;
//             }
//             node_temp = node_temp->next;
//         }
//         node_temp = table->nodes[i];
//     }
//     return NULL;
// }

list_node_t *set_insert(struct set_table *table, const char *key, const size_t key_len)
{
    size_t hash = djb33x_hash(key, key_len);
    size_t index = hash % table->hashmap_size;
    list_node_t *head = table->nodes[index];
    if (set_search(table, key, key_len))
    {
        printf("Use another key, the key you are trying to insert is already in use\n");
        return NULL;
    }
    if (!head)
    {
        table->nodes[index] = malloc(sizeof(list_node_t));
        if (!table->nodes[index])
        {
            return NULL;
        }
        key_value_t *data = malloc(sizeof(key_value_t));
        data->key = key;
        data->key_len = key_len;
        table->nodes[index]->data = (void *)data;
        table->nodes[index]->next = NULL;
        table->nodes[index]->prev = NULL;
        return table->nodes[index];
    }
    list_node_t *new_item = malloc(sizeof(list_node_t));
    if (!new_item)
    {
        return NULL;
    }
    key_value_t *data = malloc(sizeof(key_value_t));
    data->key = key;
    data->key_len = key_len;
    new_item->data = (void *)data;
    new_item->next = NULL;
    list_node_t *tail = head;
    while (head)
    {
        tail = head;
        head = head->next;
    }
    tail->next = new_item;
    new_item->prev = tail;
    return tail;
}

void print_search(list_node_t *node_to_print)
{
    if (node_to_print)
    {
        printf("Found\n-key: %s\n-key_len: %lld\n", KeyFromNode(node_to_print)->key, KeyFromNode(node_to_print)->key_len);
    }
    else
    {
        printf("Nothing found with the given key\n");
    }
}

int set_removal(set_table_t *table, const char *key, const size_t key_len)
{
    list_node_t *to_remove = set_search(table, key, key_len);
    size_t hash = djb33x_hash(key, key_len);
    size_t index = hash % table->hashmap_size;
    if (to_remove)
    {
        if (to_remove == table->nodes[index])
        {
            table->nodes[index] = to_remove->next;
            free(to_remove);
            return 0;
        }
        to_remove->prev->next = to_remove->next;
        if (to_remove->next == NULL)
        {
            return 0;
        }
        to_remove->next->prev = to_remove->prev;
        free(to_remove);
        return 0;
    }
    return -1;
}

// int set_removal(set_table_t *table, const char *key)
// {
//     set_node_t *to_remove = set_search(table, key);
//     size_t hash = djb33x_hash(to_remove->key, to_remove->key_len);
//     size_t index = hash % table->hashmap_size;
//     if (to_remove)
//     {
//         if (to_remove == table->nodes[index])
//         {
//             table->nodes[index] = to_remove->next;
//             free(to_remove);
//             return 0;
//         }
//         to_remove->prev->next = to_remove->next;
//         if (to_remove->next == NULL)
//         {
//             return 0;
//         }
//         to_remove->next->prev = to_remove->prev;
//         free(to_remove);
//         return 0;
//     }
//     return -1;
// }

int main()
{
    set_table_t *my_sets_list = set_table_new(100, data_to_key);
    // Insert
    printf("---------------INSERT---------------\n");
    set_insert(my_sets_list, "AB", 2);
    set_insert(my_sets_list, "ABC", 2);
    set_insert(my_sets_list, "ABC", 2);
    set_insert(my_sets_list, "ABC", 1);
    set_insert(my_sets_list, "ABCD", 2);
    printf("-------------END INSERT-------------\n\n");
    // length
    printf("---------------LENGTH---------------\n");
    printf("LENGTH: %lld\n", list_len(set_search(my_sets_list, "AB", 2)));
    printf("-------------END LENGTH-------------\n\n");
    // search
    printf("---------------SEARCH---------------\n");
    list_node_t *search_node_AB = set_search(my_sets_list, "AB", 2);
    print_search(search_node_AB);
    list_node_t *search_node_ABC = set_search(my_sets_list, "ABC", 2);
    print_search(search_node_ABC);
    printf("-------------END SEARCH-------------\n\n");
    // remove
    printf("---------------REMOVE---------------\n");
    // set_removal(my_sets_list, "ABC", 2);
    printf("-------------END REMOVE-------------\n\n");
    // re-print length
    printf("---------------LENGTH---------------\n");
    printf("LENGTH: %lld\n", list_len(set_search(my_sets_list, "AB", 2)));
    printf("-------------END LENGTH-------------\n\n");
    // research
    printf("---------RE-SEARCH REMOVED----------\n");
    list_node_t *search_node_ABC2 = set_search(my_sets_list, "ABC", 2);
    print_search(search_node_ABC2);
    printf("--------END RE-SEARCH REMOVED--------\n\n");
    // search further
    printf("-------SEARCH NEXT OF REMOVED--------\n");
    list_node_t *search_node_ABCD = set_search(my_sets_list, "ABCD", 2);
    print_search(search_node_ABCD);
    printf("-----END SEARCH NEXT OF REMOVED------\n\n");

    // //shuffle work but after shuffling index will be different from the hashed one
    // printf("---------------SHUFFLE--------------\n");
    // list_node_t *get_head = set_search(my_sets_list, "AB", 2);
    // list_shuffle(&get_head);
    // printf("-------------END SHUFFLE------------\n");

    // print from first in line
    printf("------------PRINT FIRST-------------\n");
    list_print(set_search(my_sets_list, "AB", 2), my_sets_list->data_to_key);
    printf("-----------END OLD PRINT------------\n\n");

    // list remove
    size_t hash = djb33x_hash("AB", 2);
    size_t index = hash % my_sets_list->hashmap_size;
    printf("------------REMOVE LIST-------------\n");
    list_remove(&my_sets_list->nodes[index], 0);
    printf("----------END REMOVE LIST-----------\n\n");

    printf("------------PRINT FIRST-------------\n");
    list_print(set_search(my_sets_list, "ABC", 2), my_sets_list->data_to_key);
    printf("-----------END OLD PRINT------------\n\n");
    return 0;
}