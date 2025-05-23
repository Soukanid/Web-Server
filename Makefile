SRC = $(wildcard *.cpp)

OBJ = $(SRC:.cpp=.o)

NAME = webserver

CXX = c++

CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -g3

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f ${OBJ}

fclean: clean
	rm -f ${NAME}
re: fclean all

.PHONY: all clean fclean re

