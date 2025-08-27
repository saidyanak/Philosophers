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

CC = gcc
CFLAGS = -Wall -Wextra -Werror -pthread #-fsanitize=thread
CFLAGS_DEBUG = -Wall -Wextra -Werror -pthread -g -fsanitize=thread -O0

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

debug: CFLAGS = $(CFLAGS_DEBUG)
debug: fclean $(OBJS)
	@echo "$(YELLOW)Linking $(NAME) with debug flags...$(NC)"
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo "$(GREEN)✅ Debug version created with Thread Sanitizer!$(NC)"

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

test: $(NAME)
	@echo "$(GREEN)🧪 Running basic tests...$(NC)"
	@echo "$(YELLOW)Test 1: Normal case$(NC)"
	./$(NAME) 5 800 200 200
	@echo "$(YELLOW)Test 2: Single philosopher$(NC)"
	./$(NAME) 1 800 200 200
	@echo "$(YELLOW)Test 3: Edge case$(NC)"
	./$(NAME) 4 310 200 100

test-deadlock: debug
	@echo "$(GREEN)🔍 Running deadlock tests...$(NC)"
	@echo "$(YELLOW)Deadlock Test 1: 2 philosophers$(NC)"
	timeout 5 ./$(NAME) 2 400 200 100 || echo "$(RED)Timeout - possible deadlock$(NC)"
	@echo "$(YELLOW)Deadlock Test 2: Edge timing$(NC)"
	timeout 5 ./$(NAME) 2 110 50 50 || echo "$(RED)Timeout - possible deadlock$(NC)"

test-stress: $(NAME)
	@echo "$(GREEN)💪 Running stress tests...$(NC)"
	@echo "$(YELLOW)Stress Test 1: Many philosophers$(NC)"
	./$(NAME) 100 800 200 200
	@echo "$(YELLOW)Stress Test 2: Fast timing$(NC)"
	./$(NAME) 10 400 100 100

valgrind: $(NAME)
	@echo "$(GREEN)🔍 Running Helgrind analysis...$(NC)"
	valgrind --tool=helgrind --log-file=helgrind.log ./$(NAME) 4 400 200 100
	@echo "$(YELLOW)Check helgrind.log for detailed analysis$(NC)"

help:
	@echo "$(GREEN)📚 Available targets:$(NC)"
	@echo "  $(YELLOW)all$(NC)          - Build the program"
	@echo "  $(YELLOW)debug$(NC)        - Build with Thread Sanitizer"
	@echo "  $(YELLOW)clean$(NC)        - Remove object files"
	@echo "  $(YELLOW)fclean$(NC)       - Remove all generated files"
	@echo "  $(YELLOW)re$(NC)           - Rebuild everything"
	@echo "  $(YELLOW)test$(NC)         - Run basic tests"
	@echo "  $(YELLOW)test-deadlock$(NC) - Run deadlock detection tests"
	@echo "  $(YELLOW)test-stress$(NC)  - Run stress tests"
	@echo "  $(YELLOW)valgrind$(NC)     - Run Helgrind analysis"
	@echo "  $(YELLOW)help$(NC)         - Show this help message"

.PHONY: all clean fclean re debug test test-deadlock test-stress valgrind help