.SILENT:


NAME = codexion
OBJ_DIR = obj

SRCS = \
		parser.c \
		utils.c \
		codexion.c \
		codexion2.c \
		codexion3.c \
		simulation.c \
		requests.c \
		error.c \
		memory_manager.c \
		heap.c \
		main.c

CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread

OBJ = $(SRCS:%.c=$(OBJ_DIR)/%.o)
HEADERS = \
		codexion.h \
		config.h \
		error.h \
		heap.h \
		memory_manager.h \
		parser.h \
		utils.h

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ)
$(OBJ_DIR)/%.o: %.c $(HEADERS) | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re