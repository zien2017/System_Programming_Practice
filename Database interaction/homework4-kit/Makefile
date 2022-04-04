CC=g++
CFLAGS=-O3
EXTRAFLAGS=-lpqxx -lpq

all:  test

test: main.cpp exerciser.h exerciser.cpp query_funcs.h query_funcs.cpp files_reader.h files_reader.cpp
	$(CC) $(CFLAGS) -o test main.cpp exerciser.cpp query_funcs.cpp files_reader.cpp $(EXTRAFLAGS)

clean:
	rm -f *~ *.o test

clobber:
	rm -f *~ *.o
