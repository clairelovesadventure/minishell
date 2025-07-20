/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_handler.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marrey <marrey@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 00:00:00 by shutan            #+#    #+#             */
/*   Updated: 2025/07/20 03:39:26 by marrey           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	count_commands(t_cmd *cmd_list)
{
	int		count;
	t_cmd	*current;

	count = 0;
	current = cmd_list;
	while (current)
	{
		count++;
		current = current->next;
	}
	return (count);
}

static void	setup_parent_signals_for_waiting(void)
{
	struct sigaction	sa;

	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}

static int	handle_child_status(int status)
{
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGQUIT)
			write(STDOUT_FILENO, "Quit (core dumped)\n", 19);
		return (128 + WTERMSIG(status));
	}
	return (1);
}

int	wait_for_children(pid_t *pids, int num_cmds, t_shell *shell)
{
	int	i;
	int	status;
	int	last_status;

	(void)shell;
	setup_parent_signals_for_waiting();
	i = 0;
	last_status = 0;
	while (i < num_cmds)
	{
		waitpid(pids[i], &status, 0);
		last_status = handle_child_status(status);
		i++;
	}
	setup_signals();
	return (last_status);
}
