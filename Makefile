.SILENT:


NAME = codexion
OBJ_DIR = obj

SRCS = \
		main.c \
		simulation.c \
		init.c \
		threads.c \
		coder.c \
		dongle.c \
		dongle2.c \
		time_utils.c \
		requests.c \
		heap.c \
		log.c \
		error.c \
		parser.c \
		memory_manager.c \
		monitoring.c \
		monitoring_checks.c \
		utils.c

CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread

OBJ = $(SRCS:%.c=$(OBJ_DIR)/%.o)
HEADERS = \
		codexion.h \
		config.h \
		error.h \
		log.h \
		heap.h \
		memory_manager.h \
		parser.h \
		utils.h \
		monitoring.h

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