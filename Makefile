# Executable name
NAME = minishell

# Directories
SRC_DIR = sources
OBJ_DIR = obj
INC_DIR = include
LIBFT_DIR = libft

# Library
LIBFT = $(LIBFT_DIR)/libft.a

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror -I$(INC_DIR) -I$(LIBFT_DIR)
LDFLAGS = -lreadline

# Find all .c files in sources/**/
SRCS = $(shell find $(SRC_DIR) -name '*.c')
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

# Create .o file paths and corresponding folders
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Link everything into final executable
$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -L$(LIBFT_DIR) -lft -o $(NAME) $(LDFLAGS)

# Build libft
$(LIBFT):
	make -C $(LIBFT_DIR)

# Targets
all: $(NAME)

clean:
	rm -rf $(OBJ_DIR)
	make clean -C $(LIBFT_DIR)

fclean: clean
	rm -f $(NAME)
	make fclean -C $(LIBFT_DIR)

re: fclean all

.PHONY: all clean fclean re
