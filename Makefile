# Compiler and Flags
CXX = g++
CXXFLAGS = -g -std=c++17 -Wall -Werror
LIBS = 

# Directories
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# Source and Object files
SRC = $(wildcard $(SRC_DIR)/*.cpp)
OBJ = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC))

# Target Executable
TARGET = $(BIN_DIR)/legv8emu

# Build Rules
all: $(TARGET)

# Link Objects into Final Executable
$(TARGET): $(OBJ) | $(BIN_DIR)
	$(CXX) $(OBJ) -o $@ $(LIBS)

# Compile Source Files into Object Files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Create Directories if Needed
$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

# Clean Rule
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)
