# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mikelzabal <mikelzabal@student.42.fr>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/15 14:20:29 by mikelzabal        #+#    #+#              #
#    Updated: 2025/04/15 14:20:36 by mikelzabal       ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# =====================================================
#               PHILO MAKEFILE ÉPICO 🔥
# =====================================================

NAME		= philo

# ===== RUTAS =====
SRC_DIR		= src
INC_DIR		= inc

# ===== ARCHIVOS =====
SRC_FILES	= main init filo_routine thanatos maitre utils simulation
SRCS		= $(addprefix $(SRC_DIR)/, $(addsuffix .c, $(SRC_FILES)))
OBJS		= $(SRCS:.c=.o)

# ===== COMPILADOR =====
CC			= cc
CFLAGS		= -Wall -Wextra -Werror -I$(INC_DIR) -pthread

# ===== COLORES =====
GREEN		= \033[0;32m
YELLOW		= \033[1;33m
BLUE		= \033[0;34m
CYAN		= \033[1;36m
RESET		= \033[0m

# ===== CONTADOR DINÁMICO =====
COMPILED	= 0

# ===== REGLAS =====

all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo ""
	@echo "$(GREEN)🎉 Compilation complete! Executable: $(NAME) 🚀$(RESET)"

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@
	$(eval COMPILED=$(shell echo $$(($(COMPILED) + 1))))
	$(eval PERCENT=$(shell echo "$$(($(COMPILED) * 100 / $(words $(SRCS))))"))
	@printf "🔧 Compiling... [$(PERCENT)%%] $(CYAN)$(notdir $<)$(RESET)\n"

clean:
	@rm -f $(OBJS)
	@echo "$(YELLOW)🧹 Object files removed$(RESET)"

fclean: clean
	@rm -f $(NAME)
	@echo "$(YELLOW)🧨 Executable removed$(RESET)"

re: fclean all

.PHONY: all clean fclean re