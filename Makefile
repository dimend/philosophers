# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dimendon <dimendon@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/16 16:45:02 by dimendon          #+#    #+#              #
#    Updated: 2025/05/30 16:28:07 by dimendon         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc
CFLAGS = -Wall -Wextra -Werror -g2 -I./includes

TARGET = philo
OBJ_PATH = obj/

SRCS = 	src/main.c \
		src/validations.c \
		src/utils.c \
		src/helpers.c \
		src/actions.c \
		src/init.c

OBJS = $(SRCS:src/%.c=$(OBJ_PATH)%.o)
HEADERS = includes/philo.h

$(OBJ_PATH)%.o: src/%.c $(HEADERS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET) -pthread

clean:
	rm -f $(OBJS)
	rm -rf $(OBJ_PATH)

fclean: clean
	rm -f $(TARGET)

re: fclean all

.PHONY: all clean fclean re
