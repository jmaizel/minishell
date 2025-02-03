NAME = minishell
TEST_NAME = tests
CC = gcc

# Compilation flags
READLINE_INC = -I/usr/local/opt/readline/include
READLINE_LIB = -L/usr/local/opt/readline/lib
CFLAGS = -Wall -Wextra -Werror -I$(INCLUDES_DIR) -I$(LIBFT_DIR) $(READLINE_INC)

# Directory structure
LIBFT_DIR = ./libft
INCLUDES_DIR = includes
OBJ_DIR = objs
PARSING_DIR = ./srcs/parsing
EXEC_DIR = ./srcs/exec
MAIN_DIR = ./srcs/main
ENV_DIR = ./srcs/env
BUILTINS_DIR = ./srcs/builtins
TOOLS_DIR = ./srcs/tools
TESTS_DIR = ./srcs/tests

# Libraries
LIBS = $(READLINE_LIB) -lreadline -L$(LIBFT_DIR) -lft
LIBFT = $(LIBFT_DIR)/libft.a

# Source files
PARSING_FILES = env.c parse_command_args.c parsing_line.c parsing_pipe.c prompt.c sep.c signals.c parsing_redir.c utils.c quotes.c count_args.c
EXEC_FILES = cleanup.c env_utils.c error_handling.c execution.c execution_utils.c expansion.c pipes.c redirection.c signals_exec.c heredoc.c exit_status.c 
MAIN_FILES = main.c
BUILTINS_FILES = echo.c cd.c pwd.c env.c export.c unset.c exit.c
TOOLS_FILES = utils.c
TEST_FILES = tests.c

SRC_FILES = $(addprefix $(MAIN_DIR)/, $(MAIN_FILES)) \
	$(addprefix $(PARSING_DIR)/, $(PARSING_FILES)) \
	$(addprefix $(EXEC_DIR)/, $(EXEC_FILES)) \
	$(addprefix $(BUILTINS_DIR)/, $(BUILTINS_FILES)) \
	$(addprefix $(TOOLS_DIR)/, $(TOOLS_FILES))

TEST_SRC_FILES = $(addprefix $(TESTS_DIR)/, $(TEST_FILES)) \
	$(addprefix $(EXEC_DIR)/, $(EXEC_FILES)) \
	$(addprefix $(BUILTINS_DIR)/, $(BUILTINS_FILES)) \
	$(addprefix $(TOOLS_DIR)/, $(TOOLS_FILES))

OBJS = $(SRC_FILES:./srcs/%.c=$(OBJ_DIR)/%.o)
TEST_OBJS = $(TEST_SRC_FILES:./srcs/%.c=$(OBJ_DIR)/%.o)

# Progress bar
TOTAL_FILES := $(words $(SRC_FILES))
COMPILED_FILES := 0

define progress_bar
	@$(eval COMPILED_FILES=$(shell echo $$(($(COMPILED_FILES) + 1))))
	@PROGRESS=$$(($(COMPILED_FILES) * 100 / $(TOTAL_FILES))); \
	BAR=$$(seq -s= $$(($$PROGRESS / 5)) | sed 's/[0-9]//g'); \
	printf "\rCompiling [%-20s] %d%%" "$$BAR" "$$PROGRESS"
endef

# Rules
all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBS)

tests: $(TEST_OBJS) $(LIBFT)
	@$(CC) $(CFLAGS) -o $(TEST_NAME) $(TEST_OBJS) $(LIBS)
	@echo "Running tests..."
	@./$(TEST_NAME)

$(OBJ_DIR)/%.o: ./srcs/%.c | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@
	$(call progress_bar)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)/parsing
	@mkdir -p $(OBJ_DIR)/exec
	@mkdir -p $(OBJ_DIR)/main
	@mkdir -p $(OBJ_DIR)/builtins
	@mkdir -p $(OBJ_DIR)/tools
	@mkdir -p $(OBJ_DIR)/tests

$(LIBFT):
	@make --no-print-directory -C $(LIBFT_DIR)

clean:
	@rm -rf $(OBJ_DIR)
	@make clean --no-print-directory -C $(LIBFT_DIR)

fclean: clean
	@rm -f $(NAME) $(TEST_NAME)
	@make fclean --no-print-directory -C $(LIBFT_DIR)

re: fclean all

.PHONY: all clean fclean re tests
