CC = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic -std=c99 -g
TARGET = baggage_priority_queue
SRCS = main.c baggage_heap.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c baggage_heap.h
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all run clean
