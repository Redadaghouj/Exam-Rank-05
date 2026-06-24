# Compiler
CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -I./given -I./complete

# Source files
SRCS = vect2.cpp main.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Executable name
TARGET = vect2

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