# Variables
CXX = g++
CXXFLAGS = -std=c++20 -I/opt/homebrew/opt/googletest/include -Iarch/node/headr -Iarch/layer/headr -I/opt/homebrew/opt/eigen/include/eigen3
LDFLAGS = -L/opt/homebrew/opt/googletest/lib -lgtest -lgtest_main -pthread

# Directories
NODE_SRC_DIR = ./arch/node/src
NODE_TEST_DIR = ./arch/node/test
LAYER_SRC_DIR = ./arch/layer/src
LAYER_TEST_DIR = ./arch/layer/test
OBJ_DIR = ./build
BIN_DIR = ./bin

# Directories creationß
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)
$(BIN_DIR):
	@mkdir -p $(BIN_DIR)

# Files
NODE_SRC = $(wildcard $(NODE_SRC_DIR)/*.cpp)
NODE_TEST_SRC = $(wildcard $(NODE_TEST_DIR)/*.cpp)
LAYER_SRC = $(wildcard $(LAYER_SRC_DIR)/*.cpp)
LAYER_TEST_SRC = $(wildcard $(LAYER_TEST_DIR)/*.cpp)

# Object files
NODE_OBJ = $(patsubst $(NODE_SRC_DIR)/%.cpp, $(OBJ_DIR)/src_node_%.o, $(NODE_SRC))
NODE_TEST_OBJ = $(patsubst $(NODE_TEST_DIR)/%.cpp, $(OBJ_DIR)/test_node_%.o, $(NODE_TEST_SRC))
LAYER_OBJ = $(patsubst $(LAYER_SRC_DIR)/%.cpp, $(OBJ_DIR)/src_layer_%.o, $(LAYER_SRC))
LAYER_TEST_OBJ = $(patsubst $(LAYER_TEST_DIR)/%.cpp, $(OBJ_DIR)/test_layer_%.o, $(LAYER_TEST_SRC))

# Target executable
TARGET = $(BIN_DIR)/run_tests

# Default target: build the executable
all: $(TARGET)

# Creating the final executable from object files
$(TARGET): $(NODE_OBJ) $(NODE_TEST_OBJ) $(LAYER_OBJ) $(LAYER_TEST_OBJ) | $(BIN_DIR)
	$(CXX) $(NODE_OBJ) $(NODE_TEST_OBJ) $(LAYER_OBJ) $(LAYER_TEST_OBJ) $(LDFLAGS) -o $@

# Rule to compile source files into object files
$(OBJ_DIR)/src_node_%.o: $(NODE_SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/test_node_%.o: $(NODE_TEST_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/src_layer_%.o: $(LAYER_SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/test_layer_%.o: $(LAYER_TEST_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean target to remove build artifacts
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Run tests
test: all
	$(TARGET)
