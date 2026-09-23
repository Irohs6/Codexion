.SILENT:


NAME = codexion
OBJ_DIR = obj

SRCS = \
		parser.c \
		utils.c \
		codexion.c \
		error.c \
		memory_manager.c \
		heap.c \
		main.c

CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread

OBJ = $(SRCS:%.c=$(OBJ_DIR)/%.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ)
$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re