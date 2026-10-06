CC = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic -std=c99 -g
TARGET = ev_network_optimizer
SRCS = main.c ev_network.c
OBJS = $(SRCS:.c=.o)
TEST_TARGET = test_suite
TEST_SRCS = test_suite.c ev_network.c
TEST_OBJS = $(TEST_SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c ev_network.h
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

test: $(TEST_TARGET)
	./$(TEST_TARGET)

$(TEST_TARGET): $(TEST_OBJS)
	$(CC) $(CFLAGS) -o $(TEST_TARGET) $(TEST_OBJS)

clean:
	rm -f $(OBJS) $(TEST_OBJS) $(TARGET) $(TEST_TARGET)

.PHONY: all run test clean
