CC = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic -std=c99 -g
TARGET = routing_analyzer
TEST_TARGET = test_runner
SRCS = main.c routing_analyzer.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c routing_analyzer.h
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

test: routing_analyzer.o test_routing_analyzer.c routing_analyzer.h
	$(CC) $(CFLAGS) -o $(TEST_TARGET) test_routing_analyzer.c routing_analyzer.o
	./$(TEST_TARGET)
	rm -f $(TEST_TARGET)

clean:
	rm -f $(OBJS) $(TARGET) $(TEST_TARGET)

.PHONY: all run test clean
