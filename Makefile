NAME = minishell
CC = gcc
CFLAGS = -Wall -Wextra -Werror -I$(INCLUDES_DIR) -I$(LIBFT_DIR) $(READLINE_INC)

LIBFT_DIR = ./libft
INCLUDES_DIR = includes
OBJ_DIR = objs

PARSING_DIR = ./srcs/parsing
EXEC_DIR = ./srcs/exec
MAIN_DIR = ./srcs/main
ENV_DIR = ./srcs/env
BUILTINS_DIR = ./srcs/builtins
TOOLS_DIR = ./srcs/tools

READLINE_INC = -I/opt/homebrew/opt/readline/include
READLINE_LIB = -L/opt/homebrew/opt/readline/lib
LIBS = $(READLINE_LIB) -lreadline -L$(LIBFT_DIR) -lft

LIBFT = $(LIBFT_DIR)/libft.a

PARSING_FILES = parsing1.c
EXEC_FILES = cleanup.c env_utils.c error_handling.c execution.c execution_utils.c expansion.c pipes.c redirection.c signals.c
MAIN_FILES = main.c
ENV_FILES =
BUILTINS_FILES = cd.c echo.c env.c exit.c pwd.c export.c unset.c
TOOLS_FILES =

SRC_FILES = $(addprefix $(MAIN_DIR)/, $(MAIN_FILES)) \
      $(addprefix $(PARSING_DIR)/, $(PARSING_FILES)) \
      $(addprefix $(EXEC_DIR)/, $(EXEC_FILES)) \
      $(addprefix $(ENV_DIR)/, $(ENV_FILES)) \
      $(addprefix $(BUILTINS_DIR)/, $(BUILTINS_FILES)) \
      $(addprefix $(TOOLS_DIR)/, $(TOOLS_FILES))

OBJS = $(SRC_FILES:./srcs/%.c=$(OBJ_DIR)/%.o)

TOTAL_FILES := $(words $(SRC_FILES))
COMPILED_FILES := 0

define progress_bar
	@$(eval COMPILED_FILES=$(shell echo $$(($(COMPILED_FILES) + 1))))
	@PROGRESS=$$(($(COMPILED_FILES) * 100 / $(TOTAL_FILES))); \
	BAR=$$(seq -s= $$(($$PROGRESS / 5)) | sed 's/[0-9]//g'); \
	printf "\rCompiling [%-20s] %d%%" "$$BAR" "$$PROGRESS"
endef

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBS)

$(OBJ_DIR)/%.o: ./srcs/%.c | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@
	$(call progress_bar)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)/parsing
	@mkdir -p $(OBJ_DIR)/exec
	@mkdir -p $(OBJ_DIR)/main
	@mkdir -p $(OBJ_DIR)/env
	@mkdir -p $(OBJ_DIR)/builtins
	@mkdir -p $(OBJ_DIR)/tools

$(LIBFT):
	@make --no-print-directory -C $(LIBFT_DIR)

clean:
	@rm -rf $(OBJ_DIR)
	@make clean --no-print-directory -C $(LIBFT_DIR)

fclean: clean
	@rm -f $(NAME)
	@make fclean --no-print-directory -C $(LIBFT_DIR)

re: fclean all

.PHONY: all clean fclean re