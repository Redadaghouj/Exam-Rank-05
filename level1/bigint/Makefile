NAME = bigint

CXX = c++ -Wall -Wextra -Werror -std=c++98

SRC = $(wildcard *.cpp)

OBJ = $(SRC:.cpp=.o)

$(NAME): $(OBJ)
	$(CXX) $(OBJ) -o $(NAME)

all: $(NAME)

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all
