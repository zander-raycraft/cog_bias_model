# Variables
CXX = g++
CXXFLAGS = -std=c++14 -I/opt/homebrew/opt/googletest/include -Iarch/node/headr .
LDFLAGS = -L/opt/homebrew/opt/googletest/lib -lgtest -lgtest_main -pthread

# Directories
NODE_SRC_DIR = ./arch/node/src            # Main source files directory
NODE_TEST_DIR = ./arch/node/test          # Test files directory
OBJ_DIR = ./build
BIN_DIR = ./bin

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)
$(BIN_DIR):
	@mkdir -p $(BIN_DIR)

# Files
NODE_SRC = $(wildcard $(NODE_SRC_DIR)/node.cpp)
NODE_TEST_SRC = $(wildcard $(NODE_TEST_DIR)/node_test.cpp)

# Object files
NODE_OBJ = $(patsubst $(NODE_SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(NODE_SRC))
NODE_TEST_OBJ = $(patsubst $(NODE_TEST_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(NODE_TEST_SRC))

# Target executable
TARGET = $(BIN_DIR)/run_tests

# Default target: build the executable
all: $(TARGET)

# Creating the final executable from object files
$(TARGET): $(NODE_OBJ) $(NODE_TEST_OBJ)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(NODE_OBJ) $(NODE_TEST_OBJ) $(LDFLAGS) -o $@

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean target to remove build artifacts
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Run tests
test: all
	$(TARGET)
