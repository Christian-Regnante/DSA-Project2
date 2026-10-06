CC = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic -std=c99 -g
TARGET = routing_analyzer
SRCS = main.c routing_analyzer.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c routing_analyzer.h
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all run clean
