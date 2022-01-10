#include ".\ToIncludeInSets\DoublyLinkedList.h"
#include <stddef.h> //required for NULL
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define list_append_item(list, item) list_append((list_node_t **)list, (list_node_t *)string_item_new(item))

typedef struct DoublyLinkedLists
{
    list_node_t *head;
    void *data_to_val;
} DoublyLinkedLists_t;

const char *data_to_val(list_node_t *node)
{
    return StringFromNode(node);
}

list_node_t *list_append(list_node_t **head, list_node_t *item)
{
    list_node_t *tail = list_get_tail(head);
    if (!tail)
    {
        *head = item;
    }
    else
    {
        tail->next = item;
    }

    item->prev = tail;
    item->next = NULL;
    return item;
}

list_node_t *string_item_new(const char *string)
{
    list_node_t *item = malloc(sizeof(list_node_t));
    if (!item)
    {
        return NULL;
    }
    memset(item, 0, sizeof(list_node_t));
    item->data = (void *)string;
    return item;
}

int list_insert(DoublyLinkedLists_t *list, size_t index, const char *string)
{
    list_node_t *temp = list->head;
    size_t len = list_len(list->head);
    if (index >= len)
    {
        return -1;
    }
    list_node_t *new_item = string_item_new(string);
    if (index == 0)
    {
        list_node_t *item = temp;
        new_item->prev = NULL;
        new_item->next = item;
        list->head = new_item;
    }

    for (size_t i = 0; i < index; i++)
    {
        temp = (list_node_t *)temp->next;
    }
    list_node_t *item = temp;
    new_item->prev = item->prev;
    new_item->next = item;
    item->prev->next = new_item;
    item->prev = new_item;
    return 0;
}

int list_insert_after(DoublyLinkedLists_t *list, const char *after, const char *string)
{
    list_node_t *temp = list->head;
    size_t len = list_len(list->head);
    while (temp != NULL && strcmp(StringFromNode(temp), after) != 0)
    {
        temp = temp->next;
    }
    if (temp == NULL)
    {
        return -1;
    }
    list_node_t *new_item = string_item_new(string);
    new_item->prev = temp;
    new_item->next = temp->next;
    if (temp->next != NULL)
    {
        temp->next->prev = new_item;
    }
    temp->next = new_item;
    return 0;
}

int list_insert_before(DoublyLinkedLists_t *list, const char *before, const char *string)
{
    list_node_t *temp = list->head;
    size_t len = list_len(list->head);
    list_node_t *prev;
    while (temp != NULL && strcmp(StringFromNode(temp), before) != 0)
    {
        prev = temp;
        temp = temp->next;
    }
    if (temp == NULL)
    {
        return -1;
    }
    list_node_t *new_item = string_item_new(string);
    new_item->prev = prev;
    new_item->next = prev->next;
    if (prev->next != NULL)
    {
        prev->next->prev = new_item;
    }
    prev->next = new_item;
    return 0;
}

DoublyLinkedLists_t *new_doubly_linked_list(const char *data_to_val(list_node_t *))
{
    DoublyLinkedLists_t *new_doubly_linked_list = malloc(sizeof(DoublyLinkedLists_t));
    if(!new_doubly_linked_list)
    {
        return NULL;
    }
    new_doubly_linked_list->head = NULL;
    new_doubly_linked_list->data_to_val = data_to_val;
    return new_doubly_linked_list;
}

int main()
{
    DoublyLinkedLists_t *my_doubly_linked_list = new_doubly_linked_list(data_to_val);
    printf("---------------INSERT---------------\n");
    list_append_item(&my_doubly_linked_list->head, "Ciao");
    list_append_item(&my_doubly_linked_list->head, "Ciao2");
    list_append_item(&my_doubly_linked_list->head, "come");
    list_append_item(&my_doubly_linked_list->head, "stai");
    list_append_item(&my_doubly_linked_list->head, "sono");
    list_append_item(&my_doubly_linked_list->head, "Andrea");
    list_append_item(&my_doubly_linked_list->head, "!!!!");
    printf("-------------END INSERT-------------\n\n");

    printf("---------------PRINT----------------\n");
    list_print(my_doubly_linked_list->head, my_doubly_linked_list->data_to_val);
    printf("-------------END PRINT--------------\n\n");

    printf("---------------REMOVE---------------\n\n");
    list_remove(&my_doubly_linked_list->head, 1);

    printf("---------------PRINT----------------\n");
    list_print(my_doubly_linked_list->head, my_doubly_linked_list->data_to_val);
    printf("-------------END PRINT--------------\n\n");

    printf("---------INSERT WITH INDEX----------\n\n");
    list_insert(my_doubly_linked_list, 1, "Giovanni");

    printf("------------INSERT AFTER------------\n\n");
    list_insert_after(my_doubly_linked_list, "Giovanni", "Rana");

    printf("------------INSERT BEFORE-----------\n\n");
    list_insert_before(my_doubly_linked_list, "come", ",");

    printf("---------------PRINT----------------\n");
    list_print(my_doubly_linked_list->head, my_doubly_linked_list->data_to_val);
    printf("-------------END PRINT--------------\n\n");

    printf("---------------SHUFFLE--------------\n");
    list_shuffle(&my_doubly_linked_list->head);
    printf("-------------END SHUFFLE------------\n\n");

    printf("---------------PRINT----------------\n");
    list_print(my_doubly_linked_list->head, my_doubly_linked_list->data_to_val);
    printf("-------------END PRINT--------------\n\n");
    return 0;
}