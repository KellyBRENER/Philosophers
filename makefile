# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kbrener- <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/02 14:01:34 by kbrener-          #+#    #+#              #
#    Updated: 2024/07/09 13:46:07 by kbrener-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo
DIR_SRC = src/
DIR_OBJ = obj/
CC = gcc
CFLAGS = -Wall -Wextra -Werror -ggdb
RM = rm -f

SRCS =  $(wildcard $(DIR_SRC)*.c) \

OBJS = $(SRCS:$(DIR_SRC)%.c=$(DIR_OBJ)%.o)

all: $(NAME)

$(DIR_OBJ)%.o: $(DIR_SRC)%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

clean:
	rm -rf $(DIR_OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all
