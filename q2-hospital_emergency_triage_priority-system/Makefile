CC = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic -std=c99 -g
TARGET = triage_priority_system
SRCS = main.c triage_heap.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c triage_heap.h
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all run clean
