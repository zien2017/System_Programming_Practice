//
// Created by Ziyuan Wang on 2022/1/12.
//
#include "my_malloc.h"

volatile size_t* LastTailPtr = NULL;
volatile size_t* LastHeadPtr = NULL;
static size_t** freeListRoot = NULL;
__thread size_t** localFreeListRoot = NULL;


static pthread_once_t malloc_once = PTHREAD_ONCE_INIT;
static pthread_mutex_t sbrk_mutex;
static sem_t freeList_r, freeList_w;
static size_t readcnt;


static void mutex_sem_init () {

    sem_init(&freeList_r, 0, 1);
    sem_init(&freeList_w, 0, 1);
    readcnt = 0;
    LastTailPtr = sbrk(MEM_ATOM_SIZE);
    LastHeadPtr = sbrk(MEM_ATOM_SIZE);
    
}


// read-write lock version (slow, no merge) version 2
void * ts_malloc_rwlock(size_t size){
    // initialize();
    pthread_once(&malloc_once, mutex_sem_init);
    void* rtn = bf_malloc(size, 2);
    return rtn;
}

void ts_free_rwlock(void *ptr){
    sem_wait(&freeList_w);
    bf_free(ptr, 2);
    sem_post(&freeList_w);
}

// simple lock version (fast, merge) version 1
void * ts_malloc_lock(size_t size){
    // initialize();
    pthread_once(&malloc_once, mutex_sem_init);
    sem_wait(&freeList_w);
    void* rtn = bf_malloc(size, 1);
    sem_post(&freeList_w);
    return rtn;
}

void ts_free_lock(void *ptr){
    sem_wait(&freeList_w);
    bf_free(ptr, 1);
    sem_post(&freeList_w);
}

//Thread Safe malloc/free: non-locking version 0
void * ts_malloc_nolock(size_t size) {
    // initialize();
    pthread_once(&malloc_once, mutex_sem_init);
    void* rtn = bf_malloc(size, 0);
    return rtn;
}

void ts_free_nolock(void *ptr) {
    bf_free(ptr, 0);
}


void freeList_reader_lock(){
    sem_wait(&freeList_r);
    readcnt ++;
    if (readcnt == 1) sem_wait(&freeList_w);
    sem_post(&freeList_r);
}

void freeList_reader_unlock(){
    sem_wait(&freeList_r);
    readcnt --;
    if (readcnt == 0) sem_post(&freeList_w);
    sem_post(&freeList_r);
}


void* ff_malloc(size_t size){
    optimizeSize(&size);
    void* ffList = FFSearchInFreeList(size);
    if (ffList != NULL) return ffList;
    return increase((size_t)size);
}



void* bf_malloc(size_t size, int version){
    optimizeSize(&size);
    void* bfList = BFSearchInFreeList(size, version);

    if (bfList != NULL) return bfList;
    
    // increase
    if (version == 2) sem_wait(&freeList_w);
    void* sbrkNew = increase((size_t)size);
    if (version == 2) sem_post(&freeList_w);
    return sbrkNew;
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

void* BFSearchInFreeList(size_t size, int version){
    
    size_t theSmallestFitSize = U_LONG_MAX;
    size_t* theSmallestFitSize_HeadPtr = NULL;
    
    // read start
    if (version == 2)  freeList_reader_lock();
    
    size_t** curr = version == 0? localFreeListRoot : freeListRoot;
    while (curr != NULL) {
        size_t* currHead = (size_t *) (curr - 1);
        if (isFreed((size_t*)currHead)) {
            if (size / MEM_ATOM_SIZE <= getSegmentSizeInSize_t((size_t *) currHead) &&
                    getSegmentSizeInSize_t((size_t *) currHead) < theSmallestFitSize) {
                theSmallestFitSize = getSegmentSizeInSize_t((size_t*)currHead);
                theSmallestFitSize_HeadPtr = (size_t*)currHead;
                if (theSmallestFitSize == size / MEM_ATOM_SIZE) {
                    break;
                }
            }
        }
        curr = (size_t**)*curr;
    }
    if (version == 2) freeList_reader_unlock();
    
    // write start
    void * splitResult = NULL;
    if (theSmallestFitSize_HeadPtr != NULL ) {
        if (version == 2) sem_wait(&freeList_w);
        if (isFreed(theSmallestFitSize_HeadPtr)) {
            splitResult = splitSegment(theSmallestFitSize_HeadPtr, size);
        }
        if (version == 2) sem_post(&freeList_w);
    }

    // unlock all

    return splitResult;
}

void removeFreeNodeFromLinkedList(void * ptr){
    // only lockversion can call this method

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
    // only lockversion can call this method
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

void ff_free(void * ptr, int version){

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

    // merge only happends in version 1
    if (! version == 1) return;

    // merge, check front
    if (isFreed(thisHead - 1)) {
        // set new free list
        removeFreeNodeFromLinkedList(ptr);

        // merge heads and tails with front
        size_t totalSize = *thisHead + *(thisHead - 1);
        *thisTail = totalSize;
        thisHead = thisTail - totalSize / MEM_ATOM_SIZE + 1;
        *thisHead = totalSize;
    }
    // merge, check behind
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

void bf_free(void * ptr, int version){
    ff_free(ptr, version);
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


size_t getSegmentSizeInSize_t(volatile const size_t* boundary_ptr){
    return *(boundary_ptr) / MEM_ATOM_SIZE;
}

size_t isFreed(volatile const size_t* boundary_ptr){
    return !(*(size_t*)boundary_ptr & U_LONG_ONE);
}

void setAllocated(volatile size_t* boundary_ptr){
    *boundary_ptr = (*(size_t*)boundary_ptr | U_LONG_ONE);
}

void setFreed(volatile size_t* boundary_ptr){
    *boundary_ptr = (*(size_t*)boundary_ptr & (U_LONG_MAX - U_LONG_ONE));
}



void* increase(size_t size){
    if (size == 0) {return NULL;}
    // size_t size = size + 2 * MEM_ATOM_SIZE;

    // alloc new mem space
    pthread_mutex_lock(&sbrk_mutex);
    size_t* sbrk_ret_ptr = (void*)sbrk(size);
    pthread_mutex_unlock(&sbrk_mutex);

    // set a new head
    *(sbrk_ret_ptr - 1) = size; // LastHeadPtr is also the new head
    setAllocated(sbrk_ret_ptr - 1);
    // set a new tail
    *(sbrk_ret_ptr - 1 + (size_t)(size / MEM_ATOM_SIZE) - 1) = size;
    setAllocated(sbrk_ret_ptr - 1 + (size_t)(size / MEM_ATOM_SIZE) - 1);

    // // reset the LastHead
    // LastHeadPtr += (size_t)(size / MEM_ATOM_SIZE);
    // *LastHeadPtr = BOUNDARY_TAG;

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
    // check if it is initialized
    if (LastTailPtr == NULL) {
        return 0;
    }
    unsigned long ans = 0;
    // go through all segments
    for (size_t* curr = (size_t*)LastTailPtr + 1;
         curr != LastHeadPtr;
         curr += getSegmentSizeInSize_t(curr)) {

        if (isFreed(curr) && *curr > ans) {
            ans = *curr;
        }
    }
    return ans;
}

unsigned long get_total_free_size(){
    // check if it is initialized
    if (LastTailPtr == NULL) {
        return 0;
    }
    unsigned long ans = 0;
    // go through all segments
    for (size_t* curr = (size_t*)LastTailPtr + 1;
         curr != LastHeadPtr;
         curr += getSegmentSizeInSize_t(curr)) {

        if (isFreed(curr)) {
            ans += *curr;
        }
    }
    return ans;
}

