NAME = libft.a

CC = gcc
CFLAGS = -Wall -Wextra -Werror

INCLUDES = -I incldudes

DIR_LIBFT = srcs/libft
DIR_PRINTF = srcs/ft_printf
DIR_GNL = srcs/get_next_line

SRC_LIBFT = $(DIR_LIBFT)/ft_atoi.c $(DIR_LIBFT)/ft_isprint.c $(DIR_LIBFT)/ft_lstlast.c \
            $(DIR_LIBFT)/ft_memmove.c $(DIR_LIBFT)/ft_strchr.c $(DIR_LIBFT)/ft_strmapi.c \
            $(DIR_LIBFT)/ft_toupper.c $(DIR_LIBFT)/ft_bzero.c $(DIR_LIBFT)/ft_itoa.c \
            $(DIR_LIBFT)/ft_lstmap.c $(DIR_LIBFT)/ft_memset.c $(DIR_LIBFT)/ft_strdup.c \
            $(DIR_LIBFT)/ft_strncmp.c $(DIR_LIBFT)/ft_calloc.c $(DIR_LIBFT)/ft_lstadd_back.c \
            $(DIR_LIBFT)/ft_lstnew.c $(DIR_LIBFT)/ft_putchar_fd.c $(DIR_LIBFT)/ft_striteri.c \
            $(DIR_LIBFT)/ft_strnstr.c $(DIR_LIBFT)/ft_isalnum.c $(DIR_LIBFT)/ft_lstadd_front.c \
            $(DIR_LIBFT)/ft_lstsize.c $(DIR_LIBFT)/ft_putendl_fd.c $(DIR_LIBFT)/ft_strjoin.c \
            $(DIR_LIBFT)/ft_strrchr.c $(DIR_LIBFT)/ft_isalpha.c $(DIR_LIBFT)/ft_lstclear.c \
            $(DIR_LIBFT)/ft_memchr.c $(DIR_LIBFT)/ft_putnbr_fd.c $(DIR_LIBFT)/ft_strlcat.c \
            $(DIR_LIBFT)/ft_strtrim.c $(DIR_LIBFT)/ft_isascii.c $(DIR_LIBFT)/ft_lstdelone.c \
            $(DIR_LIBFT)/ft_memcmp.c $(DIR_LIBFT)/ft_putstr_fd.c $(DIR_LIBFT)/ft_strlcpy.c \
            $(DIR_LIBFT)/ft_substr.c $(DIR_LIBFT)/ft_isdigit.c $(DIR_LIBFT)/ft_lstiter.c \
            $(DIR_LIBFT)/ft_memcpy.c $(DIR_LIBFT)/ft_split.c $(DIR_LIBFT)/ft_strlen.c \
            $(DIR_LIBFT)/ft_tolower.c $(DIR_LIBFT)/ft_split_pipes.c $(DIR_LIBFT)/ft_strcmp.c \
            $(DIR_LIBFT)/is_quotes.c

SRC_PRINTF = $(DIR_PRINTF)/ft_print_char.c $(DIR_PRINTF)/ft_print_int.c \
             $(DIR_PRINTF)/ft_print_str.c $(DIR_PRINTF)/ft_printf.c \
             $(DIR_PRINTF)/ft_print_hex.c $(DIR_PRINTF)/ft_print_ptr.c \
             $(DIR_PRINTF)/ft_print_unsigned_int.c

SRC_GNL = $(DIR_GNL)/get_next_line.c $(DIR_GNL)/get_next_line_bonus.c \
          $(DIR_GNL)/get_next_line_utils.c $(DIR_GNL)/get_next_line_utils_bonus.c

SRC = $(SRC_LIBFT) $(SRC_PRINTF) $(SRC_GNL)

OBJ = $(SRC:.c=.o)

TOTAL_FILES := $(words $(SRC))
COMPILED_FILES = 0

define progress_bar
	@$(eval COMPILED_FILES=$(shell echo $$(($(COMPILED_FILES) + 1))))
	@PROGRESS=$$(($(COMPILED_FILES) * 100 / $(TOTAL_FILES))); \
	BAR=$$(seq -s= $$(($$PROGRESS / 5)) | sed 's/[0-9]//g'); \
	printf "\rCompiling [%-20s] %d%%" $$BAR $$PROGRESS
endef

all: $(NAME)

$(NAME): $(OBJ)
	@ar rcs $(NAME) $(OBJ)

%.o: %.c
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
	$(call progress_bar)

clean:
	@rm -f $(OBJ)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
