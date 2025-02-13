NAME = minishell
CC = gcc

READLINE_PATH = $(shell brew --prefix readline)
READLINE_INC = -I$(READLINE_PATH)/include
READLINE_LIB = -L$(READLINE_PATH)/lib

CFLAGS = -Wall -Wextra -Werror -I$(INCLUDES_DIR) -I$(LIBFT_DIR) $(READLINE_INC)

LIBFT_DIR = ./libft
INCLUDES_DIR = includes
OBJ_DIR = objs
PARSING_DIR = ./srcs/parsing
EXEC_DIR = ./srcs/execution
MAIN_DIR = ./srcs/main
BUILTINS_DIR = ./srcs/builtins

LIBS = $(READLINE_LIB) -lreadline -L$(LIBFT_DIR) -lft
LIBFT = $(LIBFT_DIR)/libft.a

PARSING_FILES = env.c parse_command_args.c parsing_line.c parsing_pipe.c \
				prompt.c sep.c signals.c parsing_redir.c utils.c quotes.c \
				count_args.c

EXECUTION_FILES = exec.c exec_cmd.c exec_pipe.c exec_redir.c exec_utils.c \
				 exec_signals.c exec_error.c exec_heredoc.c exec_path.c exec_builtins.c

MAIN_FILES = main.c

BUILTINS_FILES = env.c cd.c echo.c pwd.c

SRC_FILES = $(addprefix $(MAIN_DIR)/, $(MAIN_FILES)) \
			$(addprefix $(PARSING_DIR)/, $(PARSING_FILES)) \
			$(addprefix $(EXEC_DIR)/, $(EXECUTION_FILES)) \
			$(addprefix $(BUILTINS_DIR)/, $(BUILTINS_FILES))

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
	@mkdir -p $(OBJ_DIR)/execution
	@mkdir -p $(OBJ_DIR)/main
	@mkdir -p $(OBJ_DIR)/builtins

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