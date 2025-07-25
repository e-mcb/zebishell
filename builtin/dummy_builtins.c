#include "../includes/minishell.h"

void	dummy_exit(t_exec *tmp, t_shell *shell)
{
	if (!tmp->arr[1])
	{
		shell->exit_status = 0;
	}
	else if (tmp->arr[1] && tmp->arr[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		shell->exit_status = 1;
	}
	else if (!ft_is_number(tmp->arr[1]))
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(tmp->arr[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		shell->exit_status = 2;
	}
	else if (ft_is_number(tmp->arr[1]))
	{
		shell->exit_status = ft_atoll(tmp->arr[1]) % 256;
		if (shell->exit_status < 0)
			shell->exit_status += 256;
	}
	return ;
}

void	dummy_cd(t_exec *tmp, t_shell *shell) //ici a verifier
{
	if (!tmp->arr[1] || tmp->arr[2])
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		shell->exit_status = 1;
	}
	else if (access(tmp->arr[1], F_OK | X_OK) != 0)
	{
		perror("minishell: cd");
		shell->exit_status = 1;
	}
	else
		shell->exit_status = 0;
	return ;
}

void	dummy_export(t_exec *tmp, t_shell *shell)
{
	if (!tmp->arr[1])
	{
		ft_print_export(shell, tmp->fd_out, ft_execsize(shell->exec));
		shell->exit_status = 0;
	}
	else if (tmp->arr[1] && !is_valid_identifier(tmp->arr[1]))
	{
		ft_putstr_fd("minishell: export: ", 2);
		ft_putstr_fd(tmp->arr[1], 2);
		ft_putstr_fd(": not a valid identifier\n", 2);
		shell->exit_status = 1;
	}
	else if (tmp->arr[1])
		shell->exit_status = 0;
}

void	dummy_unset(t_shell *shell)
{
	shell->exit_status = 0;
	return ;
}

int	check_for_dummy_builtin(t_exec *tmp, t_shell *shell)
{
	if (ft_strcmp(tmp->arr[0], "exit") == 0
		&& ft_execsize(shell->exec) > 1 && tmp->next == NULL)
	{
		dummy_exit(tmp, shell);
		return (1);
	}
	if (ft_strcmp(tmp->arr[0], "cd") == 0
		&& ft_execsize(shell->exec) > 1 && tmp->next == NULL)
	{
		dummy_cd(tmp, shell);
		return (1);
	}
	if (ft_strcmp(tmp->arr[0], "export") == 0
		&& ft_execsize(shell->exec) > 1 && tmp->next == NULL)
	{
		dummy_export(tmp, shell);
		return (1);
	}
	if (ft_strcmp(tmp->arr[0], "unset") == 0
		&& ft_execsize(shell->exec) > 1 && tmp->next == NULL)
	{
		dummy_unset(shell);
		return (1);
	}
	return (0);
}
