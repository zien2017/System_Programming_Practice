#include "exerciser.h"

void exercise(connection *C) {
    query1(C, 1, 35, 40, 0, 0, 0, 0, 5, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    query1(C, 0, 35, 40, 0, 0, 0, 1, 5, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    query2(C, "Green");
    query2(C, "Orange");
    query3(C, "Duke");
    query3(C, "UNC");
    query4(C, "MA", "Maroon");
    query4(C, "NC", "DarkBlue");
    query5(C, 13);
    query5(C, 10);

}
