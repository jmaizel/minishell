# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: votre_nom <votre_email@domain.com>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/13 14:00:00 by votre_nom         #+#    #+#              #
#    Updated: 2024/11/13 14:00:00 by votre_nom        ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Nom de la bibliothèque
NAME = libft.a

# Compilateur et options de compilation
CC = gcc
CFLAGS = -Wall -Wextra -Werror

# Répertoires
INCLUDES_DIR = includes
LIBFT_DIR = srcs/libft
PRINTF_DIR = srcs/ft_printf

# Fichiers d'en-tête
INCLUDES = -I $(INCLUDES_DIR)

# Fichiers sources de la libft
SRC_LIBFT = \
    $(LIBFT_DIR)/ft_atoi.c \
    $(LIBFT_DIR)/ft_bzero.c \
    $(LIBFT_DIR)/ft_calloc.c \
    $(LIBFT_DIR)/ft_isalnum.c \
    $(LIBFT_DIR)/ft_isalpha.c \
    $(LIBFT_DIR)/ft_isascii.c \
    $(LIBFT_DIR)/ft_isdigit.c \
    $(LIBFT_DIR)/ft_isprint.c \
    $(LIBFT_DIR)/ft_itoa.c \
    $(LIBFT_DIR)/ft_memchr.c \
    $(LIBFT_DIR)/ft_memcmp.c \
    $(LIBFT_DIR)/ft_memcpy.c \
    $(LIBFT_DIR)/ft_memmove.c \
    $(LIBFT_DIR)/ft_memset.c \
    $(LIBFT_DIR)/ft_putchar_fd.c \
    $(LIBFT_DIR)/ft_putendl_fd.c \
    $(LIBFT_DIR)/ft_putnbr_fd.c \
    $(LIBFT_DIR)/ft_putstr_fd.c \
    $(LIBFT_DIR)/ft_split.c \
    $(LIBFT_DIR)/ft_strchr.c \
    $(LIBFT_DIR)/ft_strdup.c \
    $(LIBFT_DIR)/ft_striteri.c \
    $(LIBFT_DIR)/ft_strjoin.c \
    $(LIBFT_DIR)/ft_strlcat.c \
    $(LIBFT_DIR)/ft_strlcpy.c \
    $(LIBFT_DIR)/ft_strlen.c \
    $(LIBFT_DIR)/ft_strmapi.c \
    $(LIBFT_DIR)/ft_strncmp.c \
    $(LIBFT_DIR)/ft_strnstr.c \
    $(LIBFT_DIR)/ft_strrchr.c \
    $(LIBFT_DIR)/ft_strtrim.c \
    $(LIBFT_DIR)/ft_substr.c \
    $(LIBFT_DIR)/ft_tolower.c \
    $(LIBFT_DIR)/ft_toupper.c \
    $(LIBFT_DIR)/ft_lstnew.c \
    $(LIBFT_DIR)/ft_lstadd_front.c \
    $(LIBFT_DIR)/ft_lstsize.c \
    $(LIBFT_DIR)/ft_lstlast.c \
    $(LIBFT_DIR)/ft_lstadd_back.c \
    $(LIBFT_DIR)/ft_lstdelone.c \
    $(LIBFT_DIR)/ft_lstclear.c \
    $(LIBFT_DIR)/ft_lstiter.c \
    $(LIBFT_DIR)/ft_lstmap.c

# Fichiers sources de ft_printf
SRC_PRINTF = \
    $(PRINTF_DIR)/ft_printf.c \
    $(PRINTF_DIR)/ft_print_char.c \
    $(PRINTF_DIR)/ft_print_str.c \
    $(PRINTF_DIR)/ft_print_int.c \
    $(PRINTF_DIR)/ft_print_unsigned_int.c \
    $(PRINTF_DIR)/ft_print_hex.c \
    $(PRINTF_DIR)/ft_print_ptr.c


# Tous les fichiers sources
SRC = $(SRC_LIBFT) $(SRC_PRINTF) $(SRC_GNL)

# Fichiers objets correspondants
OBJ = $(SRC:.c=.o)

# Règles du Makefile
all: $(NAME)

$(NAME): $(OBJ)
	@ar rcs $(NAME) $(OBJ)
	@echo "Libft compilée avec succès !"

%.o: %.c
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@rm -f $(OBJ)
	@echo "Nettoyage des fichiers objets."

fclean: clean
	@rm -f $(NAME)
	@echo "Nettoyage complet (objets et bibliothèque)."

re: fclean all

.PHONY: all clean fclean re
