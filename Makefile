NAME = ircserv

CC = c++
FLAGS = -Wall -Werror -Wextra -std=c++98 -I server/ -I commands/ -I client/ -g

GREEN = \033[32m
RED = \033[31m
YELLOW = \033[33m
BLUE = \033[38;2;15;101;214m
RESET = \033[0m

SRC = main.cpp server/Server.cpp commands/Command.cpp commands/Join.cpp client/Client.cpp channel/Channel.cpp

OBJS_SRC = $(SRC:.cpp=.o)


all: $(NAME)

$(NAME): $(OBJS_SRC)
	@echo ">>> $(BLUE)COMPILATION$(RESET)<<<"
	@$(CC) $(FLAGS)  $(OBJS_SRC) -o $@

%.o: %.cpp
	@$(CC) $(FLAGS) -c $< -o $@

clean:
	@echo ">>> $(RED)SUPPRESSION DES FICHIERS .o $(RESET)<<<"
	@rm -rf $(OBJS_SRC)
	@echo "$(RESET)"

fclean: clean
	@echo ">>> $(RED)SUPPRESSION DE L'EXECUTABLE $(RESET)<<<"
	@rm -f $(NAME)

re: fclean all
	@echo "$(RESET)"

.PHONY: all re fclean clean
