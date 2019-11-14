#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "list.h"

list *make_list()
{
	list *l = (list *)malloc(sizeof(list));
	l->size = 0;
	l->front = 0;
	l->back = 0;
	return l;
}

/*
void transfer_node(list *s, list *d, node *n)
{
    node *prev, *next;
    prev = n->prev;
    next = n->next;
    if(prev) prev->next = next;
    if(next) next->prev = prev;
    --s->size;
    if(s->front == n) s->front = next;
    if(s->back == n) s->back = prev;
}
*/

void *list_pop(list *l)
{
	node *b;
	void *val;
    if(!l->back) return 0;
	b = l->back;
    val = b->val;
    l->back = b->prev;
    if(l->back) l->back->next = 0;
    free(b);
    --l->size;
    
    return val;
}

void list_insert(list *l, void *val)
{
	node *new_nd = (node *)malloc(sizeof(node));
	new_nd->val = val;
	new_nd->next = 0;

	if(!l->back)
	{
		l->front = new_nd;
		new_nd->prev = 0;
	}else
	{
		l->back->next = new_nd;
		new_nd->prev = l->back;
	}
	l->back = new_nd;
	++l->size;
}
void list_insert2(list *l, list *l2)
{
	node *n = l2->front;
	while (n)
	{
		list_insert(l, n->val);
		n = n->next;
	}
}
void list_insert_t(list *l, list *l2, int num)
{
	int n0 = l->size;
	while (l->size < n0 + num)
	{
		node *n = l2->front;
		while (n && l->size < n0 + num)
		{
			assert(n->val);
			char *str = n->val;
			char *str_copy = malloc(strlen(str)+1);
			strcpy(str_copy, str);
			list_insert(l, str_copy);
			n = n->next;
		}
	}
	assert(l->size == n0 + num);
}

void free_node(node *n)
{
	node *next;
	while(n) 
	{
		next = n->next;
		free(n);
		n = next;
	}
}

void free_list(list *l)
{
	free_node(l->front);
	free(l);
}

void free_list_contents(list *l)
{
	node *n = l->front;
	while(n){
		free(n->val);
		n = n->next;
	}
}

void **list_to_array(list *l)
{
	void **a = (void **)calloc(l->size, sizeof(void*));
    int count = 0;
    node *n = l->front;
    while(n)
	{
        a[count] = n->val;
		count++;
        n = n->next;
    }
    return a;
}
