#include "vec.h"

Node *new_node(void *data)
{
    Node *node = (Node *)malloc(sizeof(Node));
    assert(node != NULL);
    node->data = data;
    node->next = NULL;
    return node;
}

void push(Vec *vec, void *data)
{
    assert(vec != NULL);
    Node *el = new_node(data);

    if (vec->head == NULL)
        vec->head = el;
    else
    {
        el->next = vec->head;
        vec->head = el;
    }

    vec->len += 1;
}

void enqueue(Vec *vec, void *data)
{
    assert(vec != NULL);
    Node *el = new_node(data);

    if (vec->head == NULL)
        vec->head = el;
    else
    {
        Node *cursor = vec->head;
        while (cursor != NULL)
        {
            if (cursor->next == NULL)
            {
                cursor->next = el;
                el->next = NULL;
            }

            cursor = cursor->next;
        }
    }
    vec->len++;
}

void insert(Vec *vec, void *data, size_t index)
{
    assert(vec != NULL && vec->len > 0 && vec->len >= index);

    if (index == 0)
    {
        push(vec, data);
        return;
    }

    Node *el = new_node(data);
    Node *cursor = vec->head;

    for (size_t i = 0; i < index - 1; ++i)
    {
        cursor = cursor->next;
    }

    el->next = cursor->next;
    cursor->next = el;
    vec->len++;
}

void *vec_remove(Vec *vec, size_t index)
{
    assert(vec != NULL && vec->len > 0 && vec->len >= index);

    if (index == 0)
    {
        return pop(vec);
    }
    else if (index == vec->len)
    {
        return dequeue(vec);
    }

    Node *cursor = vec->head;
    Node *prev = NULL;

    for (size_t i = 0; i < index; ++i)
    {
        prev = cursor;
        cursor = cursor->next;
    }
    void *result = cursor->data;
    prev->next = cursor->next;
    free(cursor);
    vec->len--;
    return result;
}

void truncate(Vec *vec, size_t len)
{
    assert(vec != NULL && vec->len > 0 && vec->len >= len);
    if (len == 0)
        return;

    if (len == vec->len)
    {
        clear(vec);
        return;
    }

    for (size_t i = 0; i < len; ++i)
    {
        dequeue(vec);
    }
}

void *dequeue(Vec *vec)
{
    assert(vec != NULL && vec->len > 0);
    if (vec->len == 1)
        return pop(vec);

    Node *cursor = vec->head;
    Node *prev = NULL;
    do
    {
        if (cursor->next == NULL)
        {
            void *result = cursor->data;
            prev->next = NULL;
            free(cursor);
            vec->len--;
            return result;
        }

        prev = cursor;
        cursor = cursor->next;
    } while (cursor != NULL);
}

void *pop(Vec *vec)
{
    assert(vec != NULL && vec->len > 0);
    Node *tmp = vec->head;
    vec->head = tmp->next;
    void *result = tmp->data;
    free(tmp);
    vec->len--;
    return result;
}

void reverse(Vec *vec)
{
    assert(vec != NULL);
    Node *prev = NULL;
    Node *current = vec->head;

    while (current != NULL)
    {
        Node *next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }

    vec->head = prev;
}

void clear(Vec *vec)
{
    assert(vec != NULL);

    Node *current = vec->head;

    while (current != NULL)
    {
        Node *tmp = current;
        current = current->next;
        free(tmp);
    }

    vec->len = 0;
}

void print_vec(Vec *vec, char type)
{
    assert(vec != NULL);
    if (vec->len == 0)
    {
        puts("[]");
        return;
    }

    Node *tmp = vec->head;
    printf("[");

    while (tmp != NULL)
    {
        switch (type)
        {
        case 'p':
            printf("%p", tmp->data);
            break;
        case 'd':
            printf("%d", (int)tmp->data);
            break;
        case 'f':
            printf("%f", tmp->data);
            break;
        case 'c':
            printf("\'%c\'", (char)tmp->data);
            break;
        case 'x':
            printf("%x", (u_int)tmp->data);
            break;
        case 'o':
            printf("%o", (u_int)tmp->data);
            break;
        case 's':
            printf("\"%s\"", (char *)tmp->data);
            break;
        default:
            fprintf(stderr, "Unsupported format especifier \'%c\'.\nAllowed formats: p, d, f, c, x, s, o\n", type);
            exit(EXIT_FAILURE);
        }
        tmp = tmp->next;
        if (tmp != NULL)
            printf(", ");
    }

    puts("]");
}

void append(Vec *dest, Vec *other)
{
    assert(dest != NULL && other != NULL && other->len > 0);

    while (other->len > 0)
    {
        enqueue(dest, pop(other));
    }
}

void *get(Vec *vec, size_t index)
{
    assert(vec != NULL && vec->len > 0 && index < vec->len);
    Node *cursor = vec->head;

    for (size_t i = 0; i < index; ++i)
    {
        cursor = cursor->next;
    }

    return cursor->data;
}

Vec clone(Vec *vec)
{
    Node *current = vec->head;
    Vec cloned = new_vec();

    while (current != NULL)
    {
        enqueue(&cloned, current->data);
        current = current->next;
    }

    return cloned;
}

Vec new_vec()
{
    Vec vec = {.len = 0, .head = NULL};
    return vec;
}