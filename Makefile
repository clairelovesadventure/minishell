NAME = push_swap

CC = cc
CFLAGS = -Wall -Wextra -Werror -I. -Iincludes

SRCS = input_check.c \
       stack_init.c \
       stack_init_a_to_b.c \
       stack_init_b_to_a.c \
       ft_split.c \
       push_swap.c \
       small_sort.c \
       sort_stacks.c \
       swap_operations.c \
       rotate_operations.c \
       reverse_operations.c \
       push_operations.c \
       push_swap_utils.c \
       push_swap_utils_cont.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -lm -o $@

%.o: %.c push_swap.h
	$(CC) $(CFLAGS) -c $< -o $@

run: $(NAME)
	./$(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re run