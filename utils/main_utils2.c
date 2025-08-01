/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzutter <mzutter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 19:46:47 by mzutter           #+#    #+#             */
/*   Updated: 2025/08/01 00:15:35 by mzutter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*generate_prompt(const char *pwd, t_shell *shell)
{
	char	*temp;
	char	*prompt;

	temp = ft_strjoin("\001\033[1;35m\002", pwd);
	if (!temp)
		ft_clean_exit(NULL, shell, NULL, NULL);
	prompt = ft_strjoin(temp, "> \001\033[0m\002");
	free(temp);
	if (!prompt)
		ft_clean_exit(NULL, shell, NULL, NULL);
	return (prompt);
}

void	get_prompt_value(char **prompt, t_shell *shell)
{
	char	cwd[1024];
	char	*pwd;

	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		pwd = ft_strdup(cwd);
		if (!pwd)
			ft_clean_exit(NULL, shell, NULL, NULL);
		*prompt = generate_prompt(pwd, shell);
		free(pwd);
	}
	else
	{
		if (!ft_getenv("PWD", shell))
			*prompt = ft_strdup("error retrieving current directory > ");
		else
		{
			pwd = ft_strdup(ft_getenv("PWD", shell));
			if (!pwd)
				ft_clean_exit(NULL, shell, NULL, NULL);
			*prompt = generate_prompt(pwd, shell);
			free(pwd);
		}
	}
}

// char	*prompt(t_shell *shell)
// {
// 	char	*input;
// 	char	*prompt;

// 	prompt = NULL;
// 	get_prompt_value(&prompt, shell);
// 	while (1)
// 	{
// 		input = readline(prompt);
// 		if (prompt)
// 			free(prompt);
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
// 			return (ft_putstr_fd(OPEN_QUOTES, 2), free(input), NULL);
// 		return (input);
// 	}
// }

char	*prompt(t_shell *shell)
{
	char	*input;
	char	*prompt;

	prompt = NULL;
	get_prompt_value(&prompt, shell);
	while (1)
	{
		input = readline(prompt);
		if (input == NULL)
		{
			free(prompt);
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
			return (free(prompt), ft_putstr_fd(OPEN_Q, 2), free(input), NULL);
		return (free(prompt), input);
	}
}
