# Compiler
CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++17 -I./given -I./complete

# Directories
GIVEN_DIR = given
COMPLETE_DIR = complete

# Source files
SRCS = \
$(GIVEN_DIR)/array_bag.cpp \
$(GIVEN_DIR)/tree_bag.cpp \
$(GIVEN_DIR)/main.cpp \
$(COMPLETE_DIR)/searchable_array_bag.cpp \
$(COMPLETE_DIR)/searchable_tree_bag.cpp \
$(COMPLETE_DIR)/set.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Executable name
TARGET = polyset

# Default rule
all: $(TARGET)

# Link
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean
clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(TARGET)

# Rebuild
re: clean all