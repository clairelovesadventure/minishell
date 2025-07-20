/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_pipeline.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marrey <marrey@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 03:26:00 by marrey            #+#    #+#             */
/*   Updated: 2025/07/20 03:29:21 by marrey           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* External function declarations */
int			create_pipes(t_cmd *cmd_list);
void		close_all_pipes(t_cmd *cmd_list);
int			count_commands(t_cmd *cmd_list);
int			setup_redirections(t_redirect *redirects, t_shell *shell);
int			execute_external_cmd(t_cmd *cmd, t_env *env_list);
void		setup_child_pipes(t_cmd *current, t_cmd *prev, t_cmd *cmd_list);
int			wait_for_children(pid_t *pids, int num_cmds, t_shell *shell);

static void	execute_child_process(t_cmd *current, t_cmd *prev,
		t_exec_data *data)
{
	reset_signals();
	setup_child_pipes(current, prev, data->cmd_list);
	if (!setup_redirections(current->redirects, data->shell))
		exit(1);
	if (current->args && current->args[0])
	{
		if (is_builtin(current->args[0]))
			exit(exec_builtin(current, data->env_list, data->shell));
		else
			exit(execute_external_cmd(current, *(data->env_list)));
	}
	exit(0);
}

static int	fork_and_execute(t_exec_data *data, pid_t *pids)
{
	t_cmd	*current;
	t_cmd	*prev;
	int		i;

	current = data->cmd_list;
	prev = NULL;
	i = 0;
	while (current)
	{
		pids[i] = fork();
		if (pids[i] == 0)
			execute_child_process(current, prev, data);
		else if (pids[i] < 0)
			return (perror("fork"), 1);
		if (prev && prev->pipe_fd[0] != -1)
			close(prev->pipe_fd[0]);
		if (prev && prev->pipe_fd[1] != -1)
			close(prev->pipe_fd[1]);
		prev = current;
		current = current->next;
		i++;
	}
	return (0);
}

static int	cleanup_pipeline_error(pid_t *pids, t_cmd *cmd_list)
{
	cleanup_temp_files(cmd_list);
	if (pids)
		free(pids);
	return (1);
}

static int	setup_pipeline_data(t_cmd *cmd_list, t_env **env_list,
		t_shell *shell, t_exec_data *data)
{
	if (preprocess_all_heredocs(cmd_list, shell) == -1)
		return (1);
	data->cmd_list = cmd_list;
	data->env_list = env_list;
	data->shell = shell;
	return (0);
}

int	execute_cmd_pipeline(t_cmd *cmd_list, t_env **env_list, t_shell *shell)
{
	pid_t		*pids;
	int			num_cmds;
	t_exec_data	data;
	int			result;

	if (setup_pipeline_data(cmd_list, env_list, shell, &data) != 0)
		return (1);
	num_cmds = count_commands(cmd_list);
	pids = malloc(sizeof(pid_t) * num_cmds);
	if (!pids || (num_cmds > 1 && !create_pipes(cmd_list)))
		return (cleanup_pipeline_error(pids, cmd_list));
	if (fork_and_execute(&data, pids) != 0)
		return (cleanup_pipeline_error(pids, cmd_list));
	close_all_pipes(cmd_list);
	result = wait_for_children(pids, num_cmds, shell);
	cleanup_temp_files(cmd_list);
	free(pids);
	return (result);
}
