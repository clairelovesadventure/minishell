# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: marrey <marrey@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/01 00:00:00 by user              #+#    #+#              #
#    Updated: 2025/05/12 22:02:50 by marrey           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror
RM = rm -f

# 包含libft库
LIBFT_PATH = ./libft
LIBFT = $(LIBFT_PATH)/libft.a

# 头文件路径
INCLUDES = -I./includes -I./libft/includes

# 源文件路径
SRCS_PATH = ./srcs

# 源文件
SRCS = $(SRCS_PATH)/main.c \
       $(SRCS_PATH)/lexer/lexer.c \
       $(SRCS_PATH)/parser/parser.c \
       $(SRCS_PATH)/expansion/expansion.c \
       $(SRCS_PATH)/expansion/variable_expansion.c \
       $(SRCS_PATH)/expansion/quote_removal.c \
       $(SRCS_PATH)/executor/executor.c \
       $(SRCS_PATH)/executor/heredoc.c \
       $(SRCS_PATH)/executor/filename_utils.c \
       $(SRCS_PATH)/builtins/echo.c \
       $(SRCS_PATH)/builtins/cd.c \
       $(SRCS_PATH)/builtins/pwd.c \
       $(SRCS_PATH)/builtins/export.c \
       $(SRCS_PATH)/builtins/unset.c \
       $(SRCS_PATH)/builtins/env.c \
       $(SRCS_PATH)/builtins/exit.c \
       $(SRCS_PATH)/signals/signals.c \
       $(SRCS_PATH)/env/env.c \
       $(SRCS_PATH)/utils/error_utils.c

# 目标文件
OBJS = $(SRCS:.c=.o)

# readline库
READLINE = -lreadline

# 编译规则
all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(READLINE) -o $(NAME)

$(LIBFT):
	make -C $(LIBFT_PATH)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(RM) $(OBJS)
	make -C $(LIBFT_PATH) clean

fclean: clean
	$(RM) $(NAME)
	make -C $(LIBFT_PATH) fclean

re: fclean all

.PHONY: all clean fclean re 