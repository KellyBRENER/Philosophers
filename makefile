# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kbrener- <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/02 14:01:34 by kbrener-          #+#    #+#              #
#    Updated: 2024/07/02 15:04:08 by kbrener-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Program name
NAME	= philosophers

# Compiler options
CC		= cc
CFLAGS	= -Wall -Wextra -Werror -g -pthread #-fsanitize=thread

# philosophers files
SRC		= 	src/philosophers.c \

OBJ		= $(SRC:.c=.o)

# Colors
RESET = \033[0m
BOLD = \033[1m
RED = \033[91m
GREEN = \033[92m
YELLOW = \033[33m
ORANGE = \033[93m
BLUE = \033[94m

all : $(NAME)

# Compiling philosophers
$(NAME): $(OBJ)
	@echo "$(BOLD)$(RED)🛠️      Compiling philosophers    🛠️$(RESET)"
	@echo "\n"
	@$(CC) $(CFLAGS) $(OBJ) -o $(NAME) -g
	@echo "$(BOLD)$(GREEN)✅✅      philosophers fully compiled, ready to use       ✅✅$(RESET)"
	@echo "\n"

clean:
	@rm -rf $(OBJ)
	@echo "$(BOLD)$(ORANGE)🌀     Cleaned .o philosophers's files   🌀$(RESET)"

fclean: clean
	@rm -rf $(NAME)
	@echo "$(BOLD)$(ORANGE)🌀     Cleaned philosophers exec       🌀$(RESET)"

re: fclean all

.PHONY: all clean fclean re bonus
