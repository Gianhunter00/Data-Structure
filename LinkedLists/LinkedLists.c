#include <stddef.h> //required for NULL
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

typedef struct list_node
{
    struct list_node *next;
} list_node_t;

typedef struct string_item
{
    list_node_t node;
    const char *string;
} string_item_t;

#define list_append_item(list, item) list_append((list_node_t **)list, (list_node_t *)string_item_new(item))

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
};

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

    item->next = NULL;
    return item;
}

list_node_t *list_pop(list_node_t **head)
{
    list_node_t *current_head = *head;
    if (!current_head)
    {
        return NULL;
    }

    *head = (*head)->next;
    current_head->next = NULL;
    return current_head;
}

size_t list_len(string_item_t *list)
{
    size_t len = 0;
    string_item_t *string_item = list;
    while (string_item)
    {
        string_item = (string_item_t *)string_item->node.next;
        len++;
    }
    return len;
}

int list_remove(string_item_t *list, size_t index)
{
    size_t len = list_len(list);
    if (index >= len)
    {
        return -1;
    }
    string_item_t *temp = list;
    if (index == 0)
    {
        *list = *(string_item_t *)list->node.next;
        free(temp);
        return 0;
    }
    size_t current_index = 0;
    string_item_t *prev_item = list;
    while (current_index < (index - 1))
    {
        prev_item = (string_item_t *)prev_item->node.next;
        current_index++;
    }
    string_item_t *current_item = (string_item_t *)prev_item->node.next;
    string_item_t *next_item = (string_item_t *)current_item->node.next;
    prev_item->node.next = &next_item->node;
    current_item->node.next = NULL;
    free(current_item);
    return 0;
}

string_item_t *string_item_new(const char *string)
{
    string_item_t *item = malloc(sizeof(string_item_t));
    if (!item)
    {
        return NULL;
    }
    item->string = string;
    return item;
}

int string_item_reverse(string_item_t **list)
{
    size_t len = list_len(*list);
    if(len == 0 || len == 1)
    {
        return -1;
    }
    size_t current_index = 0;
    size_t current_last_index = 2;
    string_item_t *temp = *list;
    string_item_t *current_item_to_append = (string_item_t *)list_get_tail((list_node_t **)*list);
    string_item_t *new_head = (string_item_t *)list_get_tail((list_node_t **)*list);
    while (current_last_index != len)
    {
        while (current_index < (len - current_last_index))
        {
            temp = (string_item_t *)temp->node.next;
            current_index++;
        }
        current_item_to_append->node.next = &temp->node;
        if (current_last_index == 2)
        {
            new_head = current_item_to_append;
        }
        current_item_to_append = temp;
        current_last_index++;
        current_index = 0;
        temp = *list;
    }
    (*list)->node.next = NULL;
    current_item_to_append->node.next = &(*list)->node;
    *list = new_head;
    return 0;
}

void list_print(string_item_t *list)
{
    string_item_t *string_item = list;
    while (string_item)
    {
        printf("Len: %s\n", string_item->string);
        string_item = (string_item_t *)string_item->node.next;
    }
}
int main()
{
    string_item_t *my_linked_list = NULL;
    printf("---------------INSERT---------------\n");
    list_append_item(&my_linked_list, "Ciao1");
    list_append_item(&my_linked_list, "Ciao2");
    list_append_item(&my_linked_list, "come");
    list_append_item(&my_linked_list, "stai");
    list_append_item(&my_linked_list, "!!!");
    list_append_item(&my_linked_list, "!!!!");
    printf("-------------END INSERT-------------\n");

    printf("---------------PRINT----------------\n");
    list_print(my_linked_list);
    printf("-------------END PRINT--------------\n");

    printf("---------------REMOVE---------------\n");
    list_remove(my_linked_list, 5);

    printf("---------------PRINT----------------\n");
    list_print(my_linked_list);
    printf("-------------END PRINT--------------\n");

    printf("---------------REVERSE--------------\n");
    string_item_reverse(&my_linked_list);
    //printf("-------------END REVERSE------------\n");
    
    printf("---------------PRINT----------------\n");
    list_print(my_linked_list);
    printf("-------------END PRINT--------------\n");
}