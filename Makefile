CC=gcc
CFLAGS=-I. -Wall
CFLAGS+=-D__DEBUG__ -g

OBJS=$(SRCS:.c=.o)

SRCS = prtfmt.c test.c
DEPS = prtfmt.h
BIN  = mini_printf

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@

all: $(BIN) tag
	./$(BIN)
#	gdb --args ./$(BIN)

run: $(BIN)
	./$(BIN)


$(BIN): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

tag:
	ctags -R

clean:
	rm -f *.o *~ $(BIN) tags

