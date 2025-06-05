CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude
LDFLAGS = -lm
SRC_DIR = src
BUILD_DIR = build
DEMO_DIR = demo

SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRC_FILES))

DEMO_SRC = $(DEMO_DIR)/test_math.c
DEMO_OBJ = $(BUILD_DIR)/test_math.o
DEMO_BIN = $(BUILD_DIR)/math_demo

.PHONY: all clean run

all: $(DEMO_BIN)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Compile source .c files into .o files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile demo/test file
$(BUILD_DIR)/test_math.o: $(DEMO_SRC) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Link everything into final binary
$(DEMO_BIN): $(OBJ_FILES) $(DEMO_OBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# Run the demo
run: $(DEMO_BIN)
	./$(DEMO_BIN)

clean:
	rm -rf $(BUILD_DIR)/*