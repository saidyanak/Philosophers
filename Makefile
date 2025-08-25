NAME = philo
CC = cc
CFLAGS = -Wall -Wextra -Werror
RM = rm -rf
OBJ = $(SRC:.c=.o)
SRC = main.c
OBJ_DIR = obj
LIBFTDIR = libft
LIBFT = $(LIBFTDIR)/libft.a
LIBFT_FLAGS = -L$(LIBFTDIR) -lft

OBJ = $(addprefix $(OBJ_DIR)/, $(SRC:.c=.o))

# Color codes
GREEN = \033[0;32m
RED = \033[0;31m
YELLOW = \033[0;33m
RESET = \033[0m

all: $(NAME)

$(NAME): $(LIBFT) $(OBJ)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(LIBFT_FLAGS)  && echo "$(GREEN)✅ Compilation successful!$(RESET)" || echo "$(RED)❌ Compilation failed!$(RESET)"

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT):
	@make -C $(LIBFTDIR)

clean:
	@rm -rf $(OBJDIR)
	@make clean -C $(LIBFTDIR)

fclean: clean
	@rm -f $(NAME)
	@make fclean -C $(LIBFTDIR)

re: fclean all

.PHONY: all clean fclean re