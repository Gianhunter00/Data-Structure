#pragma once
#include <stddef.h> //required for NULL
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

typedef struct list_node
{
    struct list_node *prev;
    struct list_node *next;
    void *data;
} list_node_t;

#define StringFromNode(node) ((const char *)(node->data))

list_node_t *list_get_tail(struct list_node **head)
{
    list_node_t *current_node = *head;
    list_node_t *last_node = NULL;

    while (current_node)
    {
        last_node = current_node;
        current_node = current_node->next;
    }
    return last_node;
}

list_node_t *list_append(list_node_t **head, list_node_t *item);

size_t list_len(list_node_t *head)
{
    size_t len = 0;
    list_node_t *node = head;
    while (node)
    {
        node = node->next;
        len++;
    }
    return len;
}

list_node_t *list_subtract(list_node_t **list, size_t index)
{
    list_node_t *temp = *list;
    size_t len = list_len(*list);
    if (index >= len)
    {
        return NULL;
    }
    if (index == 0)
    {
        *list = (*list)->next;
        temp->next = NULL;
        temp->prev = NULL;
        return temp;
    }

    for (size_t i = 0; i < index; i++)
    {
        temp = temp->next;
    }
    temp->prev->next = temp->next;
    if (temp->next == NULL)
    {
        return temp;
    }
    temp->next->prev = temp->prev;
    temp->next = NULL;
    temp->prev = NULL;
    return temp;
}

int list_remove(list_node_t **list, size_t index)
{
    list_node_t *to_free = list_subtract(list, index);
    if (to_free != NULL)
    {
        free(to_free);
        return 0;
    }
    return 1;
}

list_node_t *string_item_new(const char *string);

int valueinarray(int val, int *arr, int len)
{
    int i;
    for (i = 0; i < len; i++)
    {
        if (arr[i] == val)
        {
            return 1;
        }
    }
    return 0;
}

void list_shuffle(list_node_t **list)
{
    srand(time(NULL));
    size_t initial_len = list_len(*list);
    size_t len = 0;
    list_node_t *temp = NULL;
    size_t j = 0;
    list_node_t *first = NULL;
    for (size_t i = 0; i < initial_len; i++)
    {
        len = list_len(*list);
        j = rand() % len;
        list_node_t *t = list_subtract(list, j);
        if (i == 0)
        {
            first = t;
            temp = t;
        }
        else
        {
            temp->next = t;
            temp->next->prev = temp;
            temp = temp->next;
        }
    }
    *list = first;
}

void list_print(list_node_t *head, void *to_string(list_node_t *))
{
    list_node_t *node = head;
    while (node)
    {
        printf("Len: %s\n", (char *)to_string(node));
        node = node->next;
    }
}