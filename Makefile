CC = clang
CFLAGS = -std=c99 -Wall -Wextra -Wpedantic
SANITIZER_FLAGS = -fsanitize=address
STATIC_ANALYZER = scan-build
FORMATTER = clang-format

# Source files and output
SRC = src/main.c
OBJ = $(SRC:.c=.o)
TARGET = chessEngine

# Format configuration
FORMAT_CONFIG = .clang-format

# Default target
all: $(TARGET)

# Build target
$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(SANITIZER_FLAGS)

# Compile source files
%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Run static analysis
analyze:
	$(STATIC_ANALYZER) $(CC) $(CFLAGS) -o $(BUILD_DIR)/$(TARGET) $(SRC)

# Run with sanitizer
sanitize: $(TARGET)
	./$(TARGET)

# Format code
format:
	$(FORMATTER) -i $(SRC)

# Clean up build artifacts
clean:
	rm -rf $(TARGET) $(OBJ)

help:
	@echo "Makefile commands:"
	@echo "  make            - Build the project."
	@echo "  make analyze    - Run static analysis on the source code."
	@echo "  make sanitize   - Run the program with Address Sanitizer."
	@echo "  make format     - Format the source code using Clang-Format."
	@echo "  make clean      - Remove build artifacts."
	@echo "  make help       - Display this help message."

# Phony targets
.PHONY: all clean analyze sanitize format help
