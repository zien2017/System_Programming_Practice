//
// Created by Brandon Wong on 2022/1/12.
//

#include "my_malloc.h"

int main(void) {
    int* myInt1 = ff_malloc(sizeof (int));
    *myInt1 = 0x55555555;


    int* myInt2 = bf_malloc(sizeof (int));
    *myInt2 = 0x66666666;

    int* myLong3 = ff_malloc(sizeof (long));
    *myLong3 = 0x77777777;

    assert(*myInt1 == 0x55555555);
    assert(*myInt2 == 0x66666666);
    assert(*myLong3 == 0x77777777);


    ff_free(myInt1);
    ff_free(myLong3);


    unsigned long largest_free_data_segment_size = get_largest_free_data_segment_size();

    unsigned long total_free_size = get_total_free_size();

    ff_free(myInt2);

    largest_free_data_segment_size = get_largest_free_data_segment_size();
    total_free_size = get_total_free_size();

    int* myInt5 = ff_malloc(sizeof (int));

    *myInt5 = 0x88888888;

    int* myInt6 = ff_malloc(sizeof (int));

    *myInt6 = 0x99999999;

    largest_free_data_segment_size = get_largest_free_data_segment_size();

    total_free_size = get_total_free_size();

    return EXIT_SUCCESS;
}

