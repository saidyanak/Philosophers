# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: syanak <syanak@student.42kocaeli.com.tr    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/08/25 10:13:23 by syanak            #+#    #+#              #
#    Updated: 2025/08/25 15:04:41 by syanak           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo

CC = cc
CFLAGS = -Wall -Wextra -Werror
SRCS = main.c \
       utils.c \
       init.c \
       cleanup.c \
       monitor.c \
       actions.c \
       threads.c

OBJS = $(SRCS:.c=.o)

GREEN = \033[0;32m
RED = \033[0;31m
YELLOW = \033[0;33m
NC = \033[0m

all: $(NAME)

$(NAME): $(OBJS)
	@echo "$(YELLOW)Linking $(NAME)...$(NC)"
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo "$(GREEN)✅ $(NAME) created successfully!$(NC)"

%.o: %.c
	@echo "$(YELLOW)Compiling $<...$(NC)"
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "$(RED)🧹 Cleaning object files...$(NC)"
	@rm -f $(OBJS)

fclean: clean
	@echo "$(RED)🧹 Cleaning $(NAME)...$(NC)"
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re