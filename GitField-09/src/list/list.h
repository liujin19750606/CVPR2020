#ifndef LIST_H
#define LIST_H

typedef struct node{
    void *val;
    struct node *next;
    struct node *prev;
} node;

typedef struct list{
    int size;
    node *front;
    node *back;
} list;

#ifdef __cplusplus  
extern "C" 
{  
#endif 
list *make_list();
int list_find(list *l, void *val);

void list_insert(list *, void *);
void list_insert2(list *l, list *l2);
void list_insert_t(list *l, list *l2, int num);

void **list_to_array(list *l);

void free_list(list *l);
void free_list_contents(list *l);

#ifdef __cplusplus  
}
#endif 
#endif
