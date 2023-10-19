//
// Created by Brandon Wong on 2022/1/12.
//
#include "my_malloc.h"

size_t* LastTailPtr = NULL;
size_t* LastHeadPtr = NULL;
size_t** freeListRoot = NULL;

void* ff_malloc(size_t size){
    initialize();
    optimizeSize(&size);
    void* ffList = FFSearchInFreeList(size);
    if (ffList != NULL) return ffList;
    return increase((size_t)size);
}



void* bf_malloc(size_t size){
    initialize();
    optimizeSize(&size);
    void* bfList = BFSearchInFreeList(size);
    if (bfList != NULL) return bfList;
    return increase((size_t)size);
}

void* FFSearchInFreeList(size_t size){
    size_t** curr = freeListRoot;
    while (curr != NULL) {
        size_t* currHead = (size_t *) (curr - 1);
        if (isFreed((size_t*)currHead) && getSegmentSizeInSize_t((size_t *) currHead) >= size / MEM_ATOM_SIZE){
            return splitSegment(((size_t *) currHead), size);
        }
        curr = (size_t**)*curr;
    }
    return NULL;
}

void* BFSearchInFreeList(size_t size){
    size_t** curr = freeListRoot;
    size_t theSmallestFitSize = U_LONG_MAX;
    size_t* theSmallestFitSize_HeadPtr = NULL;
    while (curr != NULL) {
        size_t* currHead = (size_t *) (curr - 1);
        if (isFreed((size_t*)currHead)) {
            if (size / MEM_ATOM_SIZE <= getSegmentSizeInSize_t((size_t *) currHead) &&
                    getSegmentSizeInSize_t((size_t *) currHead) < theSmallestFitSize) {
                if (getSegmentSizeInSize_t((size_t*) currHead) == size / MEM_ATOM_SIZE) {
                    return splitSegment(((size_t *) currHead), size);
                }
                theSmallestFitSize = getSegmentSizeInSize_t((size_t*)currHead);
                theSmallestFitSize_HeadPtr = (size_t*)currHead;
            }
        }
        curr = (size_t**)*curr;
    }
    if (theSmallestFitSize_HeadPtr != NULL) {
        return splitSegment(theSmallestFitSize_HeadPtr, size);
    }
    return NULL;
}

void removeFreeNodeFromLinkedList(void * ptr){
    size_t** next = (size_t**)ptr;
    size_t** prev = (size_t**)ptr + 1;
    if (*next != NULL) {
        size_t** next_prev = (size_t **) (*(next) + 1);
        *next_prev = (size_t*)*prev;
    }
    if (*prev != NULL)  {
        size_t** prev_next = (size_t **) (*(prev) - 1);
        *prev_next = (size_t *) *next;
    } else {
        freeListRoot = (size_t**)*next;
    }

}

void addFreeNodeToLinkedList(void * ptr){
    size_t** next = (size_t**)ptr;
    size_t** prev = (size_t**)ptr + 1;
    if (freeListRoot != NULL) {
        *(freeListRoot + 1) = *prev;
    }
    *prev = NULL;
    *next = (size_t*)freeListRoot;
    freeListRoot = next;
    if (*next != NULL) {
        *(*(next) + 1) = (size_t)prev;
    }
}

void ff_free(void * ptr){
    // add a node to the free list
    addFreeNodeToLinkedList(ptr);
    // set myself freed
    size_t* thisHead = (size_t*)ptr - 1;
    size_t* thisTail = (size_t*)ptr + getSegmentSizeInSize_t(thisHead) - 2;
    assert (thisHead < thisTail);
    assert (!isFreed(thisHead));
    assert (!isFreed(thisTail));
    setFreed(thisHead); // set Head Freed
    setFreed(thisTail); // set Tail Freed
    // check front
    if (isFreed(thisHead - 1)) {
        // set new free list
        removeFreeNodeFromLinkedList(ptr);

        // merge heads and tails with front
        size_t totalSize = *thisHead + *(thisHead - 1);
        *thisTail = totalSize;
        thisHead = thisTail - totalSize / MEM_ATOM_SIZE + 1;
        *thisHead = totalSize;
    }
    // check behind
    if (isFreed(thisTail + 1)) {
        // set new free list
        removeFreeNodeFromLinkedList((size_t *) thisHead + getSegmentSizeInSize_t(thisHead) + 1);

        // merge heads and tails with behind
        size_t totalSize = *thisTail + *(thisTail + 1);
        *thisHead = totalSize;
        thisTail = thisHead + totalSize / MEM_ATOM_SIZE - 1;
        *thisTail = totalSize;
    }
}

void bf_free(void * ptr){
    ff_free(ptr);
}

void* splitSegment(size_t* head, size_t const size){
    size_t* tail = head + getSegmentSizeInSize_t(head) - 1;
    if (*head - size >= 4 * MEM_ATOM_SIZE) {
        addFreeNodeToLinkedList(head + size / MEM_ATOM_SIZE + 1);
        // set the other block new head
        *(head + size / MEM_ATOM_SIZE) = *head - size;
        // set the other block new tail
        *tail = *head - size;
        // set this block new head
        *head = size;
        // set this block new tail
        tail = (head + size / MEM_ATOM_SIZE - 1);
        *tail = size;
    }
    removeFreeNodeFromLinkedList(head + 1);
    setAllocated(head);
    setAllocated(tail);
    return head + 1;
}

void initialize() {
    // check if it is the first time to allocate
    if (LastTailPtr == NULL) {
        LastTailPtr = sbrk(MEM_ATOM_SIZE);
        LastHeadPtr = sbrk(MEM_ATOM_SIZE);
        *LastTailPtr = BOUNDARY_TAG;
        *LastHeadPtr = BOUNDARY_TAG;
    }
}

size_t getSegmentSizeInSize_t(const size_t* boundary_ptr){
    return *(boundary_ptr) / MEM_ATOM_SIZE;
}

size_t isFreed(const size_t* boundary_ptr){
    return !(*(size_t*)boundary_ptr & U_LONG_ONE);
}

void setAllocated(size_t* boundary_ptr){
    *boundary_ptr = (*(size_t*)boundary_ptr | U_LONG_ONE);
}

void setFreed(size_t* boundary_ptr){
    *boundary_ptr = (*(size_t*)boundary_ptr & (U_LONG_MAX - U_LONG_ONE));
}



void* increase(size_t size){
    if (size == 0) {return NULL;}
    // size_t size = size + 2 * MEM_ATOM_SIZE;

    // alloc new mem space
    void* sbrk_ret_ptr = (void*)sbrk(size);

    // set a new head
    *(LastHeadPtr) = size; // LastHeadPtr is also the new head
    setAllocated(LastHeadPtr);
    // set a new tail
    *(LastHeadPtr + (size_t)(size / MEM_ATOM_SIZE) - 1) = size;
    setAllocated(LastHeadPtr + (size_t)(size / MEM_ATOM_SIZE) - 1);

    // reset the LastHead
    LastHeadPtr += (size_t)(size / MEM_ATOM_SIZE);
    *LastHeadPtr = BOUNDARY_TAG;

    return sbrk_ret_ptr;
    // return NULL;
}

void optimizeSize(size_t* size){
    *size += 2 * MEM_ATOM_SIZE;
    if (*size <= 4 * MEM_ATOM_SIZE) {
        *size = 4 * MEM_ATOM_SIZE;
        return;
    }
    size_t remainder = *size % MEM_ATOM_SIZE;
    if (remainder != 0){
        *size += MEM_ATOM_SIZE - remainder;
    }
}

unsigned long get_largest_free_data_segment_size(){
    unsigned long result = 0;
    size_t** curr = freeListRoot;
    while (curr != NULL) {
        size_t* currHead = (size_t *) (curr - 1);
        result = result > *currHead ? result : *currHead;
        curr = (size_t**)*curr;
    }
    return result;
}

unsigned long get_total_free_size(){
    unsigned long result = 0;
    size_t** curr = freeListRoot;
    while (curr != NULL) {
        size_t* currHead = (size_t *) (curr - 1);
        result += *currHead;
        curr = (size_t**)*curr;
    }
    return result;
}

