OBJS=main.o options.o io.o

.PHONY: all

all: filestat

filestat: $(OBJS)
	gcc -g -O0 -Wall -Wextra -o filestat $(OBJS)

%.o: %.c
	gcc -g -O0 -Wall -Wextra -c -o $@ $^
