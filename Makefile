# Variables
CXX = g++
CXXFLAGS = -std=c++11 -I/opt/homebrew/opt/googletest/include
LDFLAGS = -L/opt/homebrew/opt/googletest/lib -lgtest -lgtest_main -pthread

# Directories
NODE_SRC_DIR = ./arch/node/src              # Main source files directory
NODE_TEST_DIR = ./arch/node/test            # Test files directory
OBJ_DIR = $(shell mkdir -p build && echo ./build)
BIN_DIR = $(shell mkdir -p bin && echo ./bin)

# Filesm
NODE_SRC = $(wildcard $(NODE_SRC_DIR)/*.cpp)     # All source .cpp files in the source directory
NODE_TEST_SRC = $(wildcard $(NODE_TEST_DIR)/*.cpp)  # Test .cpp files in the test directory

# Object files
NODE_OBJ = $(patsubst $(NODE_SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(notdir $(NODE_SRC)))  # Object files from main source
NODE_TEST_OBJ = $(patsubst $(NODE_TEST_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(notdir $(NODE_TEST_SRC)))  # Object files from test file

TARGET = $(BIN_DIR)/run_tests

# Default target
all: $(TARGET)

# Creating the final executable from object files
$(TARGET): $(NODE_OBJ) $(NODE_TEST_OBJ)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(OBJ_DIR)/*.o $(LDFLAGS) -o $@

# Rule to compile source files into object files
$(OBJ_DIR)/%.o: $(NODE_SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule to compile test files into object files
$(OBJ_DIR)/%.o: $(NODE_TEST_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean target to remove build artifacts
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Run tests
test: all
	$(TARGET)
