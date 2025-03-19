# Executable name
NAME = minishell

# Directories
LIBFT_DIR = libft
LEXER_DIR = lexer
PARSER_DIR = parsing
OBJ_DIR = obj

# Include directories
INCLUDES_DIR = includes
LEXER_INC = $(LEXER_DIR)/includes
PARSER_INC = $(PARSER_DIR)/includes

# Libft Library
LIBFT = $(LIBFT_DIR)/libft.a

# Source files for each module
# Adjust these file names to match your actual sources.
LEXER_SOURCES = lexer.c token.c token_utils.c test_lexer.c parse_pwd.c parse_cd.c parse_echo.c parser_utils.c parse_export.c

# Prefix the sources with their directories
LEXER_SRCS = $(addprefix $(LEXER_DIR)/sources/, $(LEXER_SOURCES))
SOURCES = $(LEXER_SRCS)

# Generate object file names in OBJ_DIR (strip directory paths)
OBJ_FILES = $(addprefix $(OBJ_DIR)/, $(notdir $(SOURCES:.c=.o)))

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror -I$(INCLUDES_DIR) -I$(LEXER_INC)

# Pattern rules for lexer sources
$(OBJ_DIR)/%.o: $(LEXER_DIR)/sources/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Pattern rules for parser sources

# Link everything together with libft to create the executable
$(NAME): $(LIBFT) $(OBJ_FILES)
	$(CC) $(CFLAGS) $(OBJ_FILES) -L$(LIBFT_DIR) -lft -o $(NAME)

# Build libft
$(LIBFT):
	make -C $(LIBFT_DIR)

# Create object directory if it doesn't exist
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Clean: remove object files and clean libft
clean:
	rm -rf $(OBJ_DIR)
	make clean -C $(LIBFT_DIR)

# fclean: full clean including executable and libft objects
fclean: clean
	rm -f $(NAME)
	make fclean -C $(LIBFT_DIR)

# Rebuild everything
re: fclean all

# Default target
all: $(NAME)

.PHONY: all clean fclean re
