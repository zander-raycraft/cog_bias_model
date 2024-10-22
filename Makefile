# Variables
CXX = g++
CXXFLAGS = -std=c++11 -I/opt/homebrew/opt/googletest/include
LDFLAGS = -L/opt/homebrew/opt/googletest/lib -lgtest -lgtest_main -pthread

# Directories
NODE_SRC_DIR = ./arch/node/src
NODE_TEST_DIR = ./arch/node/src
OBJ_DIR = ./build
BIN_DIR = ./bin

# Files
NODE_SRC = $(wildcard $(NODE_SRC_DIR)/.cpp)
NODE_TEST_SRC = $(wildcard $(NODE_SRC_DIR)/node_test.cpp)
NNODE_OBJ = $(patsubst $(NODE_SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(NODE_SRC))
NODE_TEST_OBJ = $(patsubst $(NODE_SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(NODE_TEST_SRC))

TARGET = $(BIN_DIR)/run_tests
all: $(TARGET)

# Making final exec file
$(TARGET): $(NODE_OBJ) $(NODE_TEST_OBJ)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(NODE_OBJ) $(NODE_TEST_OBJ) $(LDFLAGS) -o $@

# Source file compilation
$(OBJ_DIR)/%.o: $(NODE_SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Test file copmilation
$(OBJ_DIR)/%.o: $(NODE_TEST_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean call
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# execute tests
test: all
	$(TARGET)