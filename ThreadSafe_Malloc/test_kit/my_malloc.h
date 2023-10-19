//
// Created by Ziyuan Wang on 2022/1/12.
//
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <pthread.h>
#include <semaphore.h>

#define MEM_ATOM_SIZE sizeof(size_t)
#define U_LONG_ONE 0x1
#define U_LONG_MAX 0x0 - U_LONG_ONE
#define BOUNDARY_TAG U_LONG_MAX

#ifndef HW2_MY_MALLOC_H
#define HW2_MY_MALLOC_H

/*
Boundary tags

-------------------------------------------------------------------------------------------------------------------------
| LastTail   || head 1 |           | tail 1 || head 2 |           | tail 2 || head 3 |           | tail 3 || LastHead   |
|            ||        |           |        ||        |           |        ||        |           |        ||            |
| 0xFFFFFFFF || size 1 | content 1 | size 1 || size 2 | content 2 | size 2 || size 3 | content 3 | size 3 || 0xFFFFFFFF |
-------------------------------------------------------------------------------------------------------------------------

*/

//Thread Safe malloc: locking version
void *ts_malloc_lock(size_t size);

//Thread Safe free: locking version
void ts_free_lock(void *ptr);

//Thread Safe malloc: non-locking version 
void *ts_malloc_nolock(size_t size);

//Thread Safe free: non-locking version 
void ts_free_nolock(void *ptr);

// readlock for freelist
void freeList_reader_lock();

// release readlock for freelist 
void freeList_reader_unlock();

//First Fit malloc
void* ff_malloc(size_t size);

//Best Fit malloc
void* bf_malloc(size_t size, int version);

// ff search in free list
void* FFSearchInFreeList(size_t size);

//bf search in free list
void* BFSearchInFreeList(size_t size, int version);

// remove a node from the free LinkedList
void removeFreeNodeFromLinkedList(void * ptr);

// add a node to the free LinkedList
void addFreeNodeToLinkedList(void * ptr);

//First Fit free
void ff_free(void * ptr, int version);

//Best Fit free
void bf_free(void * ptr, int version);

// splitSegment a freed segment to allocate new memory
void* splitSegment(size_t* head, size_t size);

// increase the top of heap when there is not enough space
void* increase(size_t size);

// set the two tags, head and tail
void initialize();

// get the offset from a head or a tail, and return the offset using in size_t* (every 4-byte)
size_t getSegmentSizeInSize_t(volatile const size_t* boundary_ptr);

// check a head of a tail if this segment is freed
size_t isFreed(volatile const size_t* boundary_ptr);

// set a head of a tail that this segment is allocated
void setAllocated(volatile size_t* boundary_ptr);

// set a head of a tail that this segment is freed
void setFreed(volatile size_t* boundary_ptr);

// reset the size requiment to fit 4-byte aligned
void optimizeSize(size_t* size);

// get the largest free data segment size
unsigned long get_largest_free_data_segment_size();//in bytes

// get the total free size in all segments
unsigned long get_total_free_size();//in bytes


#endif //HW2_MY_MALLOC_H



