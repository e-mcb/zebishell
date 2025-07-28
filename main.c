/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzutter <mzutter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 11:16:07 by mzutter           #+#    #+#             */
/*   Updated: 2025/07/26 02:42:06 by mzutter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

int	g_signal = 0;

// static char	*prompt(t_shell *shell)
// {
// 	char	*input;

// 	while (1)
// 	{
// 		input = readline("minishell> ");
// 		if (input == NULL)
// 		{
// 			ft_putstr_fd("Goodbye\n", 2);
// 			ft_end_minishell(NULL, shell, NULL, NULL);
// 		}
// 		if (input[0] != '\0')
// 			add_history(input);
// 		if (input[0] == 0)
// 		{
// 			free(input);
// 			continue ;
// 		}
// 		if (ft_has_invalid_quotes(input))
// 		{
// 			ft_putstr_fd(OPEN_QUOTES, 2);
// 			free(input);
// 			return (NULL);
// 		}
// 		return (input);
// 	}
// }

// static char	*prompt(t_shell *shell)
// {
// 	char	*input;
// 	char	*pwd;
// 	char	*prompt;

// 	while (1)
// 	{
// 		pwd = ft_getenv("PWD", shell);
// 		if (!pwd)
// 			prompt = ("ta soeur> ");
// 		else
// 		{
// 			prompt = ft_strjoin("\001\033[1;35m\002", pwd);
// 			prompt = ft_strjoin(prompt, "> ");
// 			prompt = ft_strjoin(prompt, "\001\033[0m\002");
// 		}
// 		input = readline(prompt);
// 		if (input == NULL)
// 		{
// 			ft_putstr_fd("Goodbye\n", 2);
// 			ft_end_minishell(NULL, shell, NULL, NULL);
// 		}
// 		if (input[0] != '\0')
// 			add_history(input);
// 		if (input[0] == 0)
// 		{
// 			free(input);
// 			continue ;
// 		}
// 		if (ft_has_invalid_quotes(input))
// 		{
// 			ft_putstr_fd(OPEN_QUOTES, 2);
// 			free(input);
// 			return (NULL);
// 		}
// 		return (input);
// 	}
// }
static char	*prompt(t_shell *shell)
{
	char	*input;
	char	*pwd;
	char	*prompt;
	char	cwd[1024];
	
	while (1)
	{
		if (getcwd(cwd, sizeof(cwd)) != NULL)
		{
			pwd = ft_strdup(cwd);
			if (!pwd)
				ft_clean_exit(NULL, shell, NULL, NULL);
		}
		else
			pwd = ft_getenv("PWD", shell);
		if (!pwd)
			prompt = ft_strdup("erreur de détermination du répertoire actuel > ");
		else
		{
			prompt = ft_strjoin("\001\033[1;35m\002", pwd);
			prompt = ft_strjoin(prompt, "> ");
			prompt = ft_strjoin(prompt, "\001\033[0m\002");
		}
		input = readline(prompt);
		if (input == NULL)
		{
			ft_putstr_fd("Goodbye\n", 2);
			ft_end_minishell(NULL, shell, NULL, NULL);
		}
		if (input[0] != '\0')
			add_history(input);
		if (input[0] == 0)
		{
			free(input);
			continue ;
		}
		if (ft_has_invalid_quotes(input))
		{
			ft_putstr_fd(OPEN_QUOTES, 2);
			free(input);
			return (NULL);
		}
		return (input);
	}
}

static t_shell	*init_shell(t_shell *shell, char **envp)
{
	char	*shlvl_str;

	shell = malloc(sizeof(t_shell));
	if (shell == NULL)
		ft_clean_exit(NULL, NULL, NULL, NULL);
	shell->env_arr = NULL;
	shell->splitted = NULL;
	shell->token = NULL;
	shell->pwd = init_pwd(shell);
	if (!envp[0])
		env_min(shell);
	else
		shell->env = ft_env_to_list(envp, shell);
	shell->exit_status = 0;
	if (shell->env == NULL)
		ft_clean_exit(NULL, shell, NULL, NULL);
	shell->exec = NULL;
	shlvl_str = ft_getenv("SHLVL", shell);
	if (!shlvl_str)
		update_or_add("SHLVL", "1", shell, 1);
	else
		ft_set_shlvl(shell, shlvl_str);
	update_or_add("_", "]", shell, 1);
	return (shell);
}

static void	end_loop(t_shell *shell)
{
	int	i;

	i = 3;
	free_list(&shell->token);
	free_exec_list(&(shell->exec));
	free(shell->exec);
	ft_free_str_array(shell->env_arr);
	while (i < 1023)
		close(i++);
	shell->env_arr = NULL;
	g_signal = 0;
}

static void	minishell_loop(t_shell *shell)
{
	char	*input;
	t_exec	*tmp;

	while (1)
	{
		input = prompt(shell);
		if (input == NULL)
			continue ;
		if (string_error(input))
		{
			free (input);
			continue ;
		}
		ft_parsing(input, shell);
		if (token_error(shell) == 0)
		{
			create_exec(shell);
			tmp = shell->exec;
			while (tmp)
			{
				if (tmp->amb_redir)
				{
					ft_free_str_array(tmp->arr);
					tmp->arr = malloc(sizeof(char *) * 2);
					if (!tmp->arr)
						ft_clean_exit(0, shell, 0, 0);
					tmp->arr[0] = ft_strdup("false");
					tmp->arr[1] = NULL;
				}
				tmp = tmp->next;
			}
			if (g_signal != SIGINT)
			{
				env_list_to_arr(shell);
				exec_loop(shell);
			}
		}
		end_loop(shell);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	*shell;

	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGTSTP, SIG_IGN);
	printf("Welcome to minishell\n");
	(void)argc;
	(void)argv;
	shell = NULL;
	shell = init_shell(shell, envp);
	minishell_loop(shell);
}
