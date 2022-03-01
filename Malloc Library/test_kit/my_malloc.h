//
// Created by Brandon Wong on 2022/1/12.
//
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>


#define MEM_ATOM_SIZE sizeof(size_t)
#define U_LONG_ONE 0x1
#define U_LONG_MAX 0x0 - U_LONG_ONE
#define BOUNDARY_TAG U_LONG_MAX

#ifndef HW1_MY_MALLOC_H
#define HW1_MY_MALLOC_H

/*
Boundary tags

-------------------------------------------------------------------------------------------------------------------------
| LastTail   || head 1 |           | tail 1 || head 2 |           | tail 2 || head 3 |           | tail 3 || LastHead   |
|            ||        |           |        ||        |           |        ||        |           |        ||            |
| 0xFFFFFFFF || size 1 | content 1 | size 1 || size 2 | content 2 | size 2 || size 3 | content 3 | size 3 || 0xFFFFFFFF |
-------------------------------------------------------------------------------------------------------------------------

*/


//First Fit malloc
void* ff_malloc(size_t size);

//Best Fit malloc
void* bf_malloc(size_t size);

//First Fit free
void ff_free(void * ptr);

//Best Fit free
void bf_free(void * ptr);

// split a freed segment to allocate new memory
void* split(size_t* head, size_t size);

// increase the top of heap when there is not enough space
void* increase(size_t size);

// set the two tags, head and tail
void initialize();

// get the offset from a head or a tail, and return the offset using in size_t* (every 4-byte)
size_t getOffsetForSizeTPtr(size_t* boundary_ptr);

// check a head of a tail if this segment is freed
size_t isFreed(size_t* boundary_ptr);

// set a head of a tail that this segment is allocated
void setAllocated(size_t* boundary_ptr);

// set a head of a tail that this segment is freed
void setFreed(size_t* boundary_ptr);

// reset the size requiment to fit 4-byte aligned
void optimizeSize(size_t* size);

// get the largest free data segment size
unsigned long get_largest_free_data_segment_size();//in bytes

// get the total free size in all segments
unsigned long get_total_free_size();//in bytes

#endif //HW1_MY_MALLOC_H


