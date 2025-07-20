/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marrey <marrey@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 00:00:00 by shutan            #+#    #+#             */
/*   Updated: 2025/07/20 03:27:09 by marrey           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* External function declarations */
int			count_commands(t_cmd *cmd_list);
int			is_parent_builtin(const char *cmd_name);
int			execute_cmd_pipeline(t_cmd *cmd_list, t_env **env_list,
				t_shell *shell);

int	executor(t_cmd *cmd_list, t_env **env_list, t_shell *shell)
{
	int		exit_status;
	int		num_cmds;
	char	*exit_str;

	if (!cmd_list || !shell)
		return (0);
	num_cmds = count_commands(cmd_list);
	if (is_single_parent_builtin(cmd_list, num_cmds))
	{
		exit_status = execute_builtin_command(cmd_list, env_list, shell);
	}
	else
		exit_status = execute_cmd_pipeline(cmd_list, env_list, shell);
	exit_str = ft_itoa(exit_status);
	if (exit_str)
	{
		set_env_value(env_list, "?", exit_str);
		free(exit_str);
	}
	shell->exit_status = exit_status;
	return (exit_status);
}
