NAME		:= PmergeMe
SRCS		:= main.cpp PmergeMe.cpp
OBJS		:= $(SRCS:.cpp=.o)
CPP			:= c++
CPPFLAGS	:= -Wall -Wextra -Werror -std=c++98
RM			:= rm -f

all: $(NAME)

$(NAME): $(OBJS)
	$(CPP) $(CPPFLAGS) -o $(NAME) $(OBJS)

%.o: %.cpp PmergeMe.hpp
	$(CPP) $(CPPFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
