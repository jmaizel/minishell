NAME = minishell

CC = gcc
CFLAGS = -Wall -Wextra -Werror

LIBFT_DIR = ./libft
LIBFT_INCLUDES = $(LIBFT_DIR)/includes

# Dossiers des fichiers source
PARSING_DIR = ./parsing
EXEC_DIR = ./exec
MAIN_DIR = ./main

# Fichiers source par dossier
PARSING_FILES = parsing1.c
EXEC_FILES =
MAIN_FILES = main.c

# Création des listes de fichiers source et objets
SRC_FILES = $(addprefix $(PARSING_DIR)/, $(PARSING_FILES)) \
            $(addprefix $(EXEC_DIR)/, $(EXEC_FILES)) \
            $(addprefix $(MAIN_DIR)/, $(MAIN_FILES))

OBJ = $(SRC_FILES:.c=.o)

LIBFT = $(LIBFT_DIR)/libft.a

INCLUDES = -I$(LIBFT_INCLUDES) -I.

# Règles principales
all: $(NAME)

$(NAME): $(OBJ) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT) -o $(NAME)

# Compilation des objets (règle générique)
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Compilation de la libft
$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR)

# Nettoyage
clean:
	rm -f $(OBJ)
	@$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
