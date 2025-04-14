CC = cc
CFLAGS = -Wall -Wextra -Werror -g -I./includes

TARGET = philo
OBJ_PATH = obj/

SRCS = src/main.c

OBJS = $(SRCS:src/%.c=$(OBJ_PATH)%.o)
HEADERS = includes/philo.h

$(OBJ_PATH)%.o: src/%.c $(HEADERS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

clean:
	rm -f $(OBJS)
	rm -rf $(OBJ_PATH)

fclean: clean
	rm -f $(TARGET)

re: fclean all

.PHONY: all clean fclean re
