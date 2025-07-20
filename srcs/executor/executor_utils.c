/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marrey <marrey@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 00:00:00 by shutan            #+#    #+#             */
/*   Updated: 2025/07/20 01:47:51 by marrey           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_single_parent_builtin(t_cmd *cmd_list, int num_cmds)
{
	return (num_cmds == 1 && cmd_list->args && cmd_list->args[0]
		&& is_parent_builtin(cmd_list->args[0]));
}

static void	restore_file_descriptors(int stdin_backup, int stdout_backup)
{
	dup2(stdin_backup, STDIN_FILENO);
	dup2(stdout_backup, STDOUT_FILENO);
	close(stdin_backup);
	close(stdout_backup);
}

static void	handle_exit_command(int exit_status, t_shell *shell)
{
	clean_current_command(shell);
	cleanup_before_exit(shell);
	exit(exit_status);
}

static int	execute_single_builtin(t_cmd *cmd, t_env **env_list, t_shell *shell)
{
	int	exit_status;
	int	stdin_backup;
	int	stdout_backup;

	stdin_backup = dup(STDIN_FILENO);
	stdout_backup = dup(STDOUT_FILENO);
	if (stdin_backup == -1 || stdout_backup == -1)
		return (1);
	if (!setup_redirections(cmd->redirects, shell))
		exit_status = 1;
	else if (!cmd->args || !cmd->args[0])
		exit_status = 0;
	else
		exit_status = exec_builtin(cmd, env_list, shell);
	if (cmd->args[0] && ft_strcmp(cmd->args[0], "exit") == 0)
	{
		restore_file_descriptors(stdin_backup, stdout_backup);
		handle_exit_command(exit_status, shell);
	}
	restore_file_descriptors(stdin_backup, stdout_backup);
	return (exit_status);
}

int	execute_builtin_command(t_cmd *cmd_list, t_env **env_list, t_shell *shell)
{
	return (execute_single_builtin(cmd_list, env_list, shell));
}
