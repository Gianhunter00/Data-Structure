#include <stddef.h> //required for NULL
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

size_t djb33x_hash(const char *key)
{
    size_t hash = 5381;
    size_t keylen = strlen(key);
    for (size_t i = 0; i < keylen; i++)
    {
        hash = ((hash << 5) + hash) ^ key[i];
    }
    return hash;
}

typedef struct set_node
{
    const char *key;
    void *value;
} set_node_t;

typedef struct set_dictionary
{
    char *(*to_string)(void *);
    struct set_node **nodes;
    size_t hashmap_size;
} set_dictionary_t;

char *to_string_char(void *value)
{
    char *casted_val = (char *)value;
    return casted_val;
}

char *to_string_int(void *value)
{
    static char val[100];
    int casted_val = *(int *)value;
    sprintf(val, "%d", casted_val);
    return val;
}

char *to_string_double(void *value)
{
    static char val[100];
    double casted_val = *(double *)value;
    sprintf(val, "%lf", casted_val);
    return val;
}

char *to_string_float(void *value)
{
    static char val[100];
    float casted_val = *(float *)value;
    snprintf(val, sizeof(val), "%f", casted_val);
    return val;
}

size_t index_of(set_dictionary_t *dictionary, const char *key)
{
    size_t hash = djb33x_hash(key);
    size_t index = hash % dictionary->hashmap_size;
    if (index)
    {
        return index;
    }
    return 0;
}

struct set_dictionary *dictionary_dictionary_new(const size_t hashmap_size, char *to_string(void *))
{
    struct set_dictionary *dictionary = malloc(sizeof(set_dictionary_t));
    if (!dictionary)
    {
        return NULL;
    }
    dictionary->hashmap_size = hashmap_size;
    dictionary->nodes = calloc(dictionary->hashmap_size, sizeof(struct set_node *));
    if (!dictionary->nodes)
    {
        free(dictionary);
        return NULL;
    }
    dictionary->to_string = to_string;
    return dictionary;
}

set_node_t *dictionary_search(set_dictionary_t *dictionary, const char *key)
{
    size_t index = index_of(dictionary, key);
    set_node_t *node = dictionary->nodes[index];
    if (node)
    {
        if (node->key == key)
        {
            return node;
        }
    }
    return NULL;
}

set_node_t *dictionary_remap(set_dictionary_t *dictionary, set_node_t *to_remap)
{
    size_t new_index = index_of(dictionary, to_remap->key);
    struct set_node *node = dictionary->nodes[new_index];
    if (!node)
    {
        dictionary->nodes[new_index] = to_remap;
        return dictionary->nodes[new_index];
    }
    return NULL;
}

int dictionary_resize(set_dictionary_t **dictionary)
{
    int base_number = 2;
    int distance = base_number - (*dictionary)->hashmap_size;
    while (distance <= 0)
    {
        base_number *= 2;
        distance = base_number - (*dictionary)->hashmap_size;
    }
    set_dictionary_t *new_dictionary = dictionary_dictionary_new((size_t)base_number, (*dictionary)->to_string);
    if (new_dictionary == NULL)
    {
        return -1;
    }
    for (size_t i = 0; i < (*dictionary)->hashmap_size; i++)
    {
        if ((*dictionary)->nodes[i])
        {
            if (dictionary_remap(new_dictionary, (*dictionary)->nodes[i]) == NULL)
            {
                printf("another one\n");
                free(new_dictionary->nodes);
                free(new_dictionary);
                base_number *= 2;
                new_dictionary = dictionary_dictionary_new(base_number, (*dictionary)->to_string);
                i = 0;
            };
        }
    }
    free((*dictionary)->nodes);
    free(*dictionary);
    *dictionary = new_dictionary;
    return 0;
}

struct set_node *dictionary_insert(struct set_dictionary **dictionary, const char *key, void *value)
{
    size_t hash = djb33x_hash(key);
    size_t index = hash % (*dictionary)->hashmap_size;
    struct set_node *head = (*dictionary)->nodes[index];
    if (dictionary_search(*dictionary, key))
    {
        printf("key and keylen already in use!!!\n");
        return NULL;
    }
    if (head)
    {

        printf("---------------RESIZE---------------\n");
        dictionary_resize(dictionary);
        printf("-------------END RESIZE-------------\n");
        return dictionary_insert(dictionary, key, value);
    }
    if (!head)
    {
        (*dictionary)->nodes[index] = malloc(sizeof(set_node_t));
        if (!(*dictionary)->nodes[index])
        {
            return NULL;
        }
        (*dictionary)->nodes[index]->key = key;
        (*dictionary)->nodes[index]->value = value;
        return (*dictionary)->nodes[index];
    }
    return NULL;
}

struct set_node *dictionary_insert_float(struct set_dictionary **dictionary, const char *key, float value)
{
    float *val = malloc(sizeof(float));
    *val = value;
    return dictionary_insert(dictionary, key, val);
}

struct set_node *dictionary_insert_double(struct set_dictionary **dictionary, const char *key, double value)
{
    double *val = malloc(sizeof(double));
    *val = value;
    return dictionary_insert(dictionary, key, val);
}

struct set_node *dictionary_insert_int(struct set_dictionary **dictionary, const char *key, int value)
{
    int *val = malloc(sizeof(int));
    *val = value;
    return dictionary_insert(dictionary, key, val);
}

struct set_node *dictionary_insert_string(struct set_dictionary **dictionary, const char *key, char *value)
{
    return dictionary_insert(dictionary, key, value);
}

int dictionary_removal(set_dictionary_t *dictionary, const char *key)
{
    size_t index = index_of(dictionary, key);
    if (dictionary->nodes[index])
    {
        free(dictionary->nodes[index]);
        dictionary->nodes[index] = NULL;
        return 0;
    }
    return -1;
}

void print_node(set_dictionary_t *dictionary, set_node_t *node_to_print)
{
    if (node_to_print)
    {
        char *val = dictionary->to_string(node_to_print->value);
        if (val)
        {
            printf("-KEY: %s    -VAL: %s\n", node_to_print->key, val);
        }
        else
        {
            printf("ERROR");
        }
    }
    else
    {
        printf("Nothing found with the given key\n");
    }
}

void print_dictionary(set_dictionary_t *dictionary)
{
    for (size_t i = 0; i < dictionary->hashmap_size; i++)
    {
        if (dictionary->nodes[i] != NULL)
            print_node(dictionary, dictionary->nodes[i]);
    }
}

int main()
{
    set_dictionary_t *my_dictionary_list_int = dictionary_dictionary_new(10, to_string_int);
    set_dictionary_t *my_dictionary_list_float = dictionary_dictionary_new(10, to_string_float);
    set_dictionary_t *my_dictionary_list_double = dictionary_dictionary_new(10, to_string_double);
    set_dictionary_t *my_dictionary_list_string = dictionary_dictionary_new(10, to_string_char);
    // Insert
    printf("-------------INSERT INT-------------\n");
    dictionary_insert_int(&my_dictionary_list_int, "qywhh", 0);
    dictionary_insert_int(&my_dictionary_list_int, "cdwj3", 1);
    dictionary_insert_int(&my_dictionary_list_int, "xgwtj", 2);
    dictionary_insert_int(&my_dictionary_list_int, "tykit", 3);
    dictionary_insert_int(&my_dictionary_list_int, "qtryh", 4);
    dictionary_insert_int(&my_dictionary_list_int, "ABfwf", 5);
    dictionary_insert_int(&my_dictionary_list_int, "ABCqc", 6);
    dictionary_insert_int(&my_dictionary_list_int, "cdcj5", 7);
    dictionary_insert_int(&my_dictionary_list_int, "cdgac", 8);
    dictionary_insert_int(&my_dictionary_list_int, "cdwen", 9);
    dictionary_insert_int(&my_dictionary_list_int, "gtwth", 10);
    dictionary_insert_int(&my_dictionary_list_int, "CIAO", 11);
    printf("-------------END INSERT-------------\n\n");
    // insert float
    printf("-------------INSERT FLOAT------------\n");
    dictionary_insert_float(&my_dictionary_list_float, "A", 5.0);
    dictionary_insert_float(&my_dictionary_list_float, "B", 5.1);
    dictionary_insert_float(&my_dictionary_list_float, "C", 5.2);
    dictionary_insert_float(&my_dictionary_list_float, "D", 5.3);
    dictionary_insert_float(&my_dictionary_list_float, "E", 5.4);
    dictionary_insert_float(&my_dictionary_list_float, "F", 5.5);
    dictionary_insert_float(&my_dictionary_list_float, "G", 5.6);
    dictionary_insert_float(&my_dictionary_list_float, "H", 5.7);
    dictionary_insert_float(&my_dictionary_list_float, "I", 5.8);
    dictionary_insert_float(&my_dictionary_list_float, "L", 5.9);
    dictionary_insert_float(&my_dictionary_list_float, "M", 6.0);
    printf("-------------END INSERT-------------\n\n");
    // insert double
    printf("------------INSERT DOUBLE------------\n");
    dictionary_insert_double(&my_dictionary_list_double, "AB", 5.22);
    dictionary_insert_double(&my_dictionary_list_double, "ABC", 6.22);
    printf("-------------END INSERT-------------\n\n");
    // insert string
    printf("------------INSERT STRING------------\n");
    dictionary_insert_string(&my_dictionary_list_string, "AB", "Pitagora");
    dictionary_insert_string(&my_dictionary_list_string, "ABC", "Alfabeto");
    printf("-------------END INSERT-------------\n\n");
    // search
    printf("---------------SEARCH---------------\n");
    set_node_t *search_node_double_AB = dictionary_search(my_dictionary_list_double, "AB");
    print_node(my_dictionary_list_double, search_node_double_AB);
    set_node_t *search_node_string_ABC = dictionary_search(my_dictionary_list_string, "ABC");
    print_node(my_dictionary_list_string, search_node_string_ABC);
    printf("-------------END SEARCH-------------\n\n");
    // remove
    printf("---------------REMOVE---------------\n");
    dictionary_removal(my_dictionary_list_int, "CIAO");
    dictionary_removal(my_dictionary_list_float, "M");
    printf("-------------END REMOVE-------------\n\n");
    // research
    printf("---------RE-SEARCH REMOVED----------\n");
    set_node_t *search_node_int_CIAO = dictionary_search(my_dictionary_list_int, "CIAO");
    print_node(my_dictionary_list_int, search_node_int_CIAO);
    set_node_t *search_node_float_M = dictionary_search(my_dictionary_list_float, "M");
    print_node(my_dictionary_list_float, search_node_float_M);
    printf("--------END RE-SEARCH REMOVED--------\n\n");
    // print size
    printf("----------PRINT HASHMAP SIZE---------\n");
    printf("INT DICTIONARY:\n-Size: %lld\n", my_dictionary_list_int->hashmap_size);
    printf("FLOAT DICTIONARY:\n-Size: %lld\n", my_dictionary_list_float->hashmap_size);
    printf("DOUBLE DICTIONARY:\n-Size: %lld\n", my_dictionary_list_double->hashmap_size);
    printf("STRING DICTIONARY:\n-Size: %lld\n", my_dictionary_list_string->hashmap_size);
    printf("--------END PRINT HASHMAP SIZE-------\n\n");
    // print int
    printf("---------PRINT DICTIONARY INT--------\n");
    print_dictionary(my_dictionary_list_int);
    printf("------END PRINT DICTIONARY INT-------\n\n");
    // print float
    printf("--------PRINT DICTIONARY FLOAT-------\n");
    print_dictionary(my_dictionary_list_float);
    printf("-----END PRINT DICTIONARY FLOAT------\n\n");
    // print double
    printf("-------PRINT DICTIONARY DOUBLE-------\n");
    print_dictionary(my_dictionary_list_double);
    printf("-----END PRINT DICTIONARY DOUBLE-----\n\n");
    // print string
    printf("--------PRINT DICTIONARY STRING------\n");
    print_dictionary(my_dictionary_list_string);
    printf("-----END PRINT DICTIONARY STRING-----\n\n");
    return 0;
}