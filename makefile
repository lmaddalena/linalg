SRC_DIR = src
LIB_DIR = lib
BUILD_DIR = build
BIN_DIR = bin
TEST_DIR = tests

CC = clang
CFLAGS = -std=c99 -O3 -march=native -fopenmp -g -Wall -Isrc -Werror
LDFLAGS = -L$(LIB_DIR) -llogger -ldl -lm
TARGET = $(LIB_DIR)/liblinalg.a

SRCS = $(wildcard $(SRC_DIR)/**/*.c $(SRC_DIR)/*.c)
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

TEST_SRCS=$(wildcard tests/*_tests.c)
TESTS=$(patsubst %.c,%,$(TEST_SRCS))

all: $(TARGET) tests

$(TARGET): $(OBJS) 
	@mkdir -p $(BIN_DIR)
	ar rcs $@ $^
	ranlib $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(TEST_DIR)/%_tests: $(TEST_DIR)/%_tests.c $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS) -llinalg

tests: $(TESTS)
	@sh ./tests/runtests.sh

clean:
	@rm -rf $(BUILD_DIR)/* $(BIN_DIR)/*
	@rm -rf $(TESTS)
	@echo "Clean completed."

.PHONY: all tests clean