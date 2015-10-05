/*
 * mm_alloc.h
 *
 * Exports a clone of the interface documented in "man 3 malloc".
 */

#pragma once

#ifndef _malloc_H_
#define _malloc_H_

 /* Define the block size since the sizeof will be wrong */
#define BLOCK_SIZE 40

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>

void* mm_malloc(size_t size);
void* mm_realloc(void* ptr, size_t size);
void mm_free(void* ptr);


typedef struct s_block *s_block_ptr;

/* block struct */
struct s_block {
    size_t size;
    struct s_block *next;
    struct s_block *prev;
    int free;
    void *ptr;
    /* A pointer to the allocated block */
    char data [0];
 };

/* Split block according to size, b must exist */
void split_block (s_block_ptr c, size_t s){

 s_block_ptr new;
new = (s_block_ptr)(c->data + s);
new->size = c->size - s - BLOCK_SIZE;
new->next = c->next;
new->prev = c;
new->free = 1;
new->ptr = new->data;
c->size = s;
c->next = new;
if(new->next) new->next->prev = new;

}

/* Try fusing block with neighbors */
s_block_ptr fusion(s_block_ptr c){
	 if(c->next && c->next->free){
	c->size += BLOCK_SIZE + c->next->size;
	c->next = c->next->next;
	if (c->next) c->next->prev = c;
	}
	return c;
}

/* Get the block from addr */
s_block_ptr get_block (void *p){
char *tmp;
tmp = p;
return (p = tmp -= BLOCK_SIZE);
}

/* Add a new block at the of heap,
 * return NULL if things go wrong
 */


s_block_ptr first = {0,NULL,NULL,1,NULL};
s_block_ptr last = {0,NULL,NULL,1,NULL};


s_block_ptr extend_heap (s_block_ptr last , size_t s){
	 size_t sc;
	s_block_ptr c;
	c = sbrk(0);
	sc = (size_t)sbrk(BLOCK_SIZE + s);
	if(sc < 0) return NULL;
	c->size = s;
	c->next = NULL;
	c->prev = last;
	c->ptr = c->data;
	if(last) last->next = c;
	c->free = 0;
	return c;

}


void set_block_content(s_block_ptr c,size_t s,struct s_block *next,struct s_block *prev, int free, void *ptr){
	c->size = s;
	c->next = next;
	c->prev = prev;
	c->free = free;
	c->ptr = ptr;
}


#ifdef __cplusplus
}

#endif
#endif
