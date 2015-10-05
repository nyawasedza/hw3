/*
 * mm_alloc.c
 *
 * Stub implementations of the mm_* routines. Remove this comment and provide
 * a summary of your allocator's design here.
 */

#include "mm_alloc.h"
#define align4(x) ((((x) - 1) >> 2) << 2) + 4
#include <stdlib.h>

/* Your final implementation should comment out this macro. */
#define MM_USE_STUBS
#define BLOCK_SIZE 20

void *base = NULL;

s_block_ptr extend_heap(s_block_ptr last, size_t size);
s_block_ptr find_block(s_block_ptr last, size_t size);
s_block_ptr get_block(void *p);
void split_block(s_block_ptr c, size_t size);
void copy_block(s_block_ptr src, s_block_ptr dst);


void copy_block(s_block_ptr src, s_block_ptr dst){
	int *sdata, *ddata;
	size_t i;
	sdata = src->ptr;
	ddata = dst->ptr;
	for(i = 0; i * 4 < src->size && i * 4 < dst->size; i++)
	ddata[i] = sdata[i];
}


s_block_ptr find_block(s_block_ptr last, size_t size){
	s_block_ptr c = base;
	while (c && !(c->free && c->size >= size)){
	last = c;
	c = c->next;
	}
	return c;
}
void* mm_malloc(size_t size)
{
	 s_block_ptr c, last;
	size_t s;
	s = align4(size);
	if(base){
	last = base;
	c = find_block(last, s);
	if(c){
	if((c->size - s) >= (BLOCK_SIZE + 4)) split_block(c, s);
	c->free = 0;
	}
	else{
	c = extend_heap(last, s);
	if(!c) return NULL;
	}
	}
	else{
	c = extend_heap(NULL, s);
	if(!c) return NULL;
	base = c;
	}
	return c->data;
}

void* mm_realloc(void* ptr, size_t size)
{
	 size_t s;
	s_block_ptr c, new;
	void *newp;
	if(!ptr) return (malloc(size));
	if (valid_addr(ptr)){
	s = align4(size);
	c = get_block(ptr);
	if (c->size >= s){
	if(c->size - s >= (BLOCK_SIZE + 4)) split_block(c, s);
	}
	else{
	if(c->next && c->next->free && (c->size + BLOCK_SIZE + c->next->size) >= s){
	fusion(c);
	if(c->size - s >= (BLOCK_SIZE + 4)) split_block(c, s);
	}
	else{
	newp = malloc(s);
	if (!newp) return NULL;
	new = get_block(newp);
	copy_block(c, new);
	free(ptr);
	return newp;
	}
	}
	return ptr;
	}
	return NULL;
}

int valid_addr(void *p){
	if (base){
	if(p > base && p < sbrk(0)) return (p == (get_block(p))->ptr);
	}
	return 0;
}

void mm_free(void* ptr)
{

	s_block_ptr c;
	if (valid_addr(ptr)){
	c = get_block(ptr);
	c->free = 1;
	if(c->prev && c->prev->free) c = fusion(c->prev);
	if(c->next){
	fusion(c);
	}
	else{
	if(c->prev) c->prev->next = NULL;
	else base = NULL;
	brk(c);
	}
	}
}
