# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: welepy <welepy@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/12 14:34:20 by welepy            #+#    #+#              #
#    Updated: 2025/02/18 12:32:28 by welepy           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC				=	cc
NAME			=	minishell
LRFLAGS			=	-lreadline
CFLAGS			=	-Wall -Wextra -Werror

SRCS			=	./srcs/env/expand.c						\
					./srcs/env/convert_env.c				\
					./srcs/parser/parse_utils/utils.c		\
					./srcs/parser/split/split_input_ext.c	\
					./srcs/parser/split/split_input.c		\
					./srcs/parser/lexer/tokens.c			\
					./srcs/pipe/pipe.c						\
					./srcs/utils/utils.c					\
					./srcs/utils/counters.c					\
					./srcs/utils/identifiers.c				\
					./srcs/init.c							\
					./srcs/repl/repl.c						\
					./srcs/repl/repl_utils.c				\
					./srcs/builtins/ft_echo.c				\
					./srcs/builtins/ft_pwd.c				\
					./srcs/builtins/ft_cd.c					\
					./srcs/builtins/ft_exit.c				\
					./srcs/builtins/ft_export.c				\
					./srcs/builtins/ft_unset.c				\
					./srcs/builtins/ft_env.c				\
					./srcs/minishell.c

HEADER			=	minishell.h
LIBFT			=	./libft
LFT				=	$(LIBFT)/libft.a
OBJ_DIR			=	objects
OBJS			=	$(SRCS:./%.c=$(OBJ_DIR)/%.o)

TOTAL_FILES		=	$(words $(OBJS))
CURRENT_FILE	=	0

all: $(NAME)

LIBMAKE			=	$(MAKE) -C $(LIBFT)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)/srcs/env $(OBJ_DIR)/srcs/parser/parse_utils $(OBJ_DIR)/srcs/parser/split $(OBJ_DIR)/srcs/parser/lexer $(OBJ_DIR)/srcs/utils $(OBJ_DIR)/srcs $(OBJ_DIR)/srcs/repl $(OBJ_DIR)/srcs/builtins $(OBJ_DIR)/srcs/pipe $(OBJ_DIR)/srcs/repl/read

$(OBJ_DIR)/%.o: ./%.c | $(OBJ_DIR)
	$(eval CURRENT_FILE := $(shell echo $$(($(CURRENT_FILE)+1))))
	@printf "\033[33m[Building] [%-3d%%] %-50s\r" $(shell echo $$((100 * $(CURRENT_FILE) / $(TOTAL_FILES)))) "$@"
	@$(CC) $(CFLAGS) -I. -c $< -o $@

$(NAME): $(OBJS)
	@echo "\033[34m[Linking] $(NAME)\033[0m"
	@$(MAKE) -C $(LIBFT)
	@$(CC) $(OBJS) -L$(LIBFT) -lft $(LRFLAGS) $(CFLAGS) -o $(NAME)
	@echo "\033[32m[Success] $(NAME) has been created!\033[0m"

clean:
	@rm -rf $(OBJ_DIR)
	@echo "\033[31m[Cleaned] Object files removed.\033[0m"
	@$(LIBMAKE) clean

fclean: clean
	@rm -f $(NAME)
	@echo "\033[31m[Cleaned] $(NAME) removed.\033[0m"
	@$(LIBMAKE) fclean

re: fclean all

.PHONY: all clean fclean re LIBMAKE
