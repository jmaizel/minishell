NAME = minishell

CC = gcc
CFLAGS = -Wall -Wextra -Werror

LIBFT_DIR = ./libft
LIBFT_INCLUDES = $(LIBFT_DIR)/includes



# Dossiers des fichiers source
PARSING_DIR = ./srcs/parsing
EXEC_DIR = ./srcs/exec
MAIN_DIR = ./srcs/main
ENV_DIR = ./srcs/env
BUILTINS_DIR = ./srcs/builtins
TOOLS_DIR = ./srcs/tools

# Fichiers source par dossier
PARSING_FILES = parsing1.c
EXEC_FILES = execution.c redirection.c execution_utils.c 
MAIN_FILES = main.c
BUILTINS_FILES = 
ENV_FILES = 
TOOLS_FILES =

# Création des listes de fichiers source et objets
SRC_FILES = $(addprefix $(PARSING_DIR)/, $(PARSING_FILES)) \
            $(addprefix $(EXEC_DIR)/, $(EXEC_FILES)) \
            $(addprefix $(MAIN_DIR)/, $(MAIN_FILES)) \
            $(addprefix $(ENV_DIR)/, $(ENV_FILES)) \
            $(addprefix $(BUILTINS_DIR)/, $(BUILTINS_FILES)) \
			$(addprefix $(TOOLS_DIR)/, $(TOOLS_FILES))

OBJ = $(SRC_FILES:.c=.o)

LIBFT = $(LIBFT_DIR)/libft.a

INCLUDES = -I$(LIBFT_INCLUDES) -I./includes

# Règles principales
all: $(NAME)
	@echo "SRC_FILES = $(SRC_FILES)"
	@echo "OBJ = $(OBJ)"

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
