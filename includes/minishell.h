/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzutter <mzutter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 13:31:47 by mzutter           #+#    #+#             */
/*   Updated: 2025/07/24 00:36:08 by mzutter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <stdbool.h>
# include <limits.h>
# include "libft/libft.h"

# define MAX_FD 1023
# define MAX_HEREDOC 16
# define OPEN_QUOTES "Minishell does not support open quotes,\
						please escape them properly\n"
# define DOUBLE_DOLLARS "Minishell does not support $$\n"
# define AMB_REDIR "minishell: Ambiguous redirect: "
# define PIPE_FIRST_LAST "minishell: syntax error near '|'\n"
# define NOCLOBBER "minishell: noclobber not accounted for in minishell\n\
						syntax error near token pair '>|'\n"
# define OPERATOR_EXTREMITY "minishell: Operator detected at the end of input\n"
# define SUCCESSIVE_OPERATORS "minishell: Successive operators detected \n"
# define PATH "PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:\
/sbin:/bin"
# define EOF_HEREDOC "minishell: warning:\
here-document delimited by end-of-file (wanted '"
# define SUCCESSIVE_PIPES "minishell: \
Minishell does not support successive pipes\n"

extern int	g_signal;

typedef enum e_token_type
{
	WORD, //0
	PIPE, //1
	IN, //2
	OUT, //3
	APPEND, //4
	HDOC, //5
	FD, // 6
	CMD, // 7
	ARG, // 8
	LIMITER, // 9
	FILEN, // 10
}	t_token_type;

typedef struct s_token
{
	t_token_type	type;
	int				rank;
	char			*value;
	struct s_token	*next;
	bool			amb_redir;
	bool			in_quotes;
}	t_token;

typedef struct s_quote_state
{
	int		w_start;
	bool	in_quotes;
	char	opening_quote;
}	t_quote_state;

typedef struct s_input_context
{
	char	*str;
	int		rank;
}	t_input_context;

typedef struct s_envvar
{
	char			*var;
	int				exported;
	struct s_envvar	*next;
}	t_envvar;

typedef struct s_exec
{
	char			**arr;
	int				fd_in;
	int				fd_out;
	char			*heredoc;
	bool			heredoc_bool;
	struct s_exec	*next;
	int				pid;
	bool			amb_redir;
}	t_exec;

typedef struct s_shell
{
	char		**env_arr;
	t_envvar	*env;
	t_token		*token;
	char		**splitted;
	int			exit_status;
	t_exec		*exec;
}	t_shell;

typedef struct s_expand
{
	int		i;
	int		j;
	int		count;
	int		start;
	bool	in_single_quote;
	bool	in_double_quote;
	char	**result;
	// bool	to_split;
}	t_expand;

typedef struct s_splitter
{
	size_t	start;
	size_t	capacity;
	size_t	i;
	size_t	count;
}	t_splitter;

//boolean utils functions
bool		is_quote(char c);
bool		is_closing_quote(char c, char opening_quote);
bool		handle_quotes(char c, bool *in_quotes, char *opening_quote);
bool		is_whitespace(char c);
bool		is_redir(t_token *t);

//string_utils functions
char		*ft_substrword(char *str, int start, int end, t_shell *shell);
char		*trim_quotes(char *str, t_shell *shell);
char		**ft_strdup_array(char **src, t_shell *shell);
char		*ft_strncpy(char *dest, const char *src, unsigned int n);
char		*strndup_custom(const char *s, size_t n, t_shell *shell);
int			count_strings(char **arr);
void		ft_init_var(size_t *i, size_t *count, bool *in_quotes, char *c);
void		whitespace_to_space(char *str);
int			ft_strcmp(char *s1, char *s2);
char		*ft_strndup(char *s, size_t n);
char		*remove_quotes(char *input, t_shell *shell);
void		remove_nested_quotes(char *str);

//misc utils
int			is_valid_identifier(char *str);

//env utils
int			env_var_exists(char *var, t_shell *shell);
void		update_env(char *var, char *str, t_shell *shell);
void		update_or_add(char *var, char *str, t_shell *shell, int exported);
char		*ft_getenv(char *var, t_shell *shell);
t_envvar	*ft_env_to_list(char **envp, t_shell *shell);
t_envvar	*copy_env_list(t_envvar *env, t_shell *shell);
void		ft_sort_env_list(t_envvar *head);
void		free_env_list(t_envvar **head);
int			add_env_var(t_envvar **head, char *str, int exported,
				t_shell *shell);
void		free_env_list(t_envvar **head);
void		env_list_to_arr(t_shell *shell);
int			envvar_match(char *env_var, char *var, size_t len, char *full_var);

//parsing utils
int			ft_has_invalid_quotes(const char *str);
int			string_error(char *input);
int			token_error(t_shell *shell);
void		ft_parsing(char *input, t_shell *shell);

//main lexer functions
char		**ft_split2(char const *s);
void		tokenizer(t_shell *shell, int rank);
void		refine_token_type(t_token *token);
void		second_refine_token_type(t_token *token, t_shell *shell);

//linked_list_utils functions
t_token		*create_token(char *str, t_shell *shell);
int			add_token(t_shell *shell, char *str, t_token_type type, int rank);
void		cleanup_token(char **expanded, char ***splitted);
t_token		*new_token_append(t_token *head, char *str,
				t_token_type type, t_shell *shell);
t_envvar	*create_env_var(char *str, int exported, t_shell *shell);

//builtins
int			ft_export(char **str, t_shell *shell, int exec_size, int fd_out);
int			ft_cd(char **str, t_shell *shell);
int			ft_echo(t_exec **exec, t_shell *shell, int exec_size, int fd_out);
int			ft_env(char **str, t_shell *shell, int exec_size, int fd_out);
int			ft_exit(char **arr, t_shell *shell, int exec_size);
int			ft_pwd(int fd_out, int exec_size);
int			ft_unset(char **str, t_shell *shell, int exec_size);
int			is_out_of_range(const char *nptr);

//clean exit
void		ft_free_str_array(char **arr);
void		free_list(t_token **head);
void		ft_clean_exit(char *input, t_shell *shell,
				char *str_to_free, char **arr_to_free);
void		free_exec_list(t_exec **exec);
void		ft_clean_without_exit(t_shell *shell);

//debug utils
void		ft_print_arr(char **arr);

//expand
void		expand(t_shell *shell);
int			ft_count_segments(char *input);
char		**split_and_expand(char *input, t_shell *shell);
char		*join_chars(char **str, t_shell *shell);
void		case_only_dollar(t_expand *ex);
void		case_question_mark(t_expand *ex, t_shell *shell);
void		case_env_var(t_expand *ex, char *input, t_shell *shell);
t_token		*skip_to_pipe(t_token *token);
int			is_case_only_dollar(char *input, t_expand *ex);

//tmp
char		**split_keep_separators(const char *s, bool (*is_sep)(char),
				t_shell *shell);
void		init_splitter(t_splitter *splitter);

//exec
char		*pathfinder(t_shell *shell, t_exec *current);
void		exec_loop(t_shell *shell);
char		*do_heredoc(const t_token *token, t_shell *shell);
t_token		*handle_redir(t_exec *exec, t_token *tmp, t_shell *shell);
void		create_exec(t_shell *shell);
void		execute_command(t_shell *shell, t_exec *tmp);
pid_t		safe_fork(t_shell *shell);
int			safe_pipe(int *pipe_fd, t_shell *shell);
int			is_valid_command(t_exec *tmp);
void		handle_child_process(t_shell *shell, t_exec *tmp, int *pipe_fd);
int			ft_is_builtin(char *str);
int			call_builtin(t_shell *shell, t_exec *cur_exec, char *cmd);
int			handle_builtin(t_shell *shell, t_exec *tmp);
void		setup_redirection(t_exec *tmp, int *pipe_fd, t_shell *shell);
void		handle_heredoc_input(t_exec *tmp, int *pipe_fd);
int			ft_execsize(t_exec *exec);
int			should_run_single_builtin(t_shell *shell, t_exec *cmd);
void		handle_in_child(t_shell *shell, t_exec *cmd, int *pipe_fd);
pid_t		execute_all_commands(t_shell *shell, t_exec *tmp, int *pipe_fd,
				int prev_fd_in);

void		wait_for_children_to_exit(t_shell *shell, pid_t last_pid);
void		wait_for_heredoc_to_exit(pid_t pid, t_shell *shell);
int			is_str_digit(char *str);
void		sigint_handler(int sig);
void		ft_end_minishell(char *input, t_shell *shell,
				char *str_to_free, char **arr_to_free);
void		ft_set_shlvl(t_shell *shell, char *shlvl_str);
void		env_min(t_shell *shell);
int			ft_isspace(int c);

int			check_for_dummy_builtin(t_exec *tmp, t_shell *shell);
void		dummy_exit(t_exec *tmp, t_shell *shell);
void		dummy_cd(t_exec *tmp, t_shell *shell);
void		dummy_export(t_exec *tmp, t_shell *shell);
void		dummy_unset(t_shell *shell);
void		ft_print_export(t_shell *shell, int fd_out, int exec_size);
int			ft_exit_argcheck(int c);
int			ft_is_number(const char *str);

#endif