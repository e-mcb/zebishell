# Compiler and flags
CC = cc
CFLAGS = -Wall -Wextra -Werror -Iheaders -Ilibft -g
LDFLAGS = -lreadline

# Directories
UTILS_DIR = utils
TOKENIZER_DIR = tokenizer
EXPAND_DIR = expand
BUILTIN_DIR = builtin
LIBFT_DIR = includes/libft
OBJ_DIR = objects
EXEC_DIR = exec
INPUT_CHECK_DIR = input_check

# Source files
SRCS = main.c \
	$(UTILS_DIR)/bool_utils.c \
	$(UTILS_DIR)/clean_exit.c \
	$(UTILS_DIR)/linked_list_utils.c \
	$(UTILS_DIR)/parsing_utils.c \
	$(UTILS_DIR)/string_utils.c \
	$(UTILS_DIR)/string_utils2.c \
	$(UTILS_DIR)/split_with_sep.c \
	$(UTILS_DIR)/env_utils.c \
	$(UTILS_DIR)/env_utils2.c \
	$(UTILS_DIR)/env_utils3.c \
	$(UTILS_DIR)/exec_utils1.c \
	$(UTILS_DIR)/exec_utils2.c \
	$(UTILS_DIR)/string_utils3.c \
	$(UTILS_DIR)/main_utils.c \
	$(UTILS_DIR)/main_utils2.c \
	$(UTILS_DIR)/free_utils.c \
	$(UTILS_DIR)/quotes_utils.c \
	$(UTILS_DIR)/exit_builtin_utils.c \
	$(UTILS_DIR)/debug_utils.c \
	$(UTILS_DIR)/export_utils.c \
	$(TOKENIZER_DIR)/tokenizer.c \
	$(TOKENIZER_DIR)/split2.c \
	$(TOKENIZER_DIR)/refine_token.c \
	$(TOKENIZER_DIR)/second_refine.c \
	$(EXPAND_DIR)/countsegments.c \
	$(EXPAND_DIR)/expand_cases.c \
	$(EXPAND_DIR)/expand.c \
	$(EXPAND_DIR)/joinexpanded.c \
	$(EXPAND_DIR)/splitandexpand.c \
	$(BUILTIN_DIR)/ft_cd.c \
	$(BUILTIN_DIR)/ft_echo.c \
	$(BUILTIN_DIR)/ft_env.c \
	$(BUILTIN_DIR)/ft_exit.c \
	$(BUILTIN_DIR)/ft_export.c \
	$(BUILTIN_DIR)/ft_pwd.c \
	$(BUILTIN_DIR)/ft_unset.c \
	$(BUILTIN_DIR)/dummy_builtins.c \
	$(EXEC_DIR)/exec_to_sort.c \
	$(EXEC_DIR)/pathfinder.c \
	$(EXEC_DIR)/prep_exec.c \
	$(EXEC_DIR)/redirs.c \
	$(EXEC_DIR)/heredocs.c \
	$(EXEC_DIR)/executing.c \
	$(EXEC_DIR)/exec_loop_utils.c \
	$(INPUT_CHECK_DIR)/check_input_str.c \
	$(INPUT_CHECK_DIR)/check_input_token.c \

# Object files
OBJS = $(SRCS:%.c=$(OBJ_DIR)/%.o)

# Executable names
NAME = minishell
TMP_NAME = /tmp/$(NAME)

# Default target: build both executables
all: $(NAME) $(TMP_NAME)

# Build main executable
$(NAME): $(OBJS)
	$(MAKE) -C $(LIBFT_DIR)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT_DIR)/libft.a -o $(NAME) $(LDFLAGS)

# Copy to /tmp
$(TMP_NAME): $(NAME)
	@echo "Copying $(NAME) to /tmp"
	cp $(NAME) $(TMP_NAME)

# Compile source files to object files
$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean object files
clean:
	$(MAKE) -C $(LIBFT_DIR) clean
	rm -rf $(OBJ_DIR)

# Full clean (includes binaries)
fclean: clean
	$(MAKE) -C $(LIBFT_DIR) fclean
	rm -f $(NAME) $(TMP_NAME)

# Rebuild everything
re: fclean all

.PHONY: all clean fclean re
