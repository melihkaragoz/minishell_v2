NAME = minishell

SRCS =src/execute/execute.c \
src/execute/pipes.c \
src/execute/heredoc.c \
src/parse/parse.c \
src/parse/parse_list.c \
src/builtin/builtin.c \
src/builtin/builtin_helper.c \
src/builtin/builtin_utils.c \
src/parse/parse_env.c \
src/parse/parse_access.c \
src/parse/parse_nodes.c \
src/parse/parse_utils.c \
src/parse/parse_command.c\
src/parse/parse_command_list.c\
src/parse/parse_redirection_list.c\
src/utils/utils.c \
src/utils/smart.c \
src/utils/env.c \
src/utils/parse_utils.c \
src/utils/init.c\
src/utils/signal.c\
src/main.c

OBJS  = $(SRCS:.c=.o)

LIBFT_DIR = libft

INC = -I include -I $(LIBFT_DIR)

LIBFT = $(LIBFT_DIR)/libft.a

CC = gcc

CFLAGS = -Wall -Werror -Wextra $(INC) -g

LDFLAGS = -lreadline

all: $(NAME)
	@echo "\033[32mCOMPILE HAS DONE\033[0m"

$(NAME): $(OBJS) $(LIBFT)
	@$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) $(LIBFT) -o $(NAME)

$(LIBFT):
	@make -C libft/

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -rf $(OBJS)
	@make clean -C libft/
	@echo "\033[31mREMOVED WITH CLEAN\033[0m"

fclean: clean
	@rm -rf $(NAME)
	@make fclean -C libft/
	@echo "\033[31mREMOVED WITH FCLEAN\033[0m"

re: fclean all

.PHONY: all clean fclean re
