CFLAGS=-std=gnu99 -ggdb3
PROGS=ringmaster player

all: $(PROGS)

ringmaster:ringmaster.o
	gcc -o $@ $^

player:player.o
	gcc -o $@ $^

%.o: %.c %.h
	gcc $(CFLAGS) -c $<

ringmaster.o:socket_select_server.h message_wrapper.h potato.h

player.o:socket_client.h message_wrapper.h potato.h

clean:
	rm -f $(PROGS)
	rm -f *.o
