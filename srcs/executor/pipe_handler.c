/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marrey <marrey@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 09:30:19 by marrey            #+#    #+#             */
/*   Updated: 2025/07/15 13:45:21 by marrey           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	create_pipes(t_cmd *cmd_list)
{
	t_cmd	*current;

	current = cmd_list;
	while (current && current->next)
	{
		if (pipe(current->pipe_fd) == -1)
		{
			perror("pipe");
			return (0);
		}
		current = current->next;
	}
	return (1);
}

void	close_all_pipes(t_cmd *cmd_list)
{
	t_cmd	*current;

	current = cmd_list;
	while (current)
	{
		if (current->pipe_fd[0] != -1)
		{
			close(current->pipe_fd[0]);
			current->pipe_fd[0] = -1;
		}
		if (current->pipe_fd[1] != -1)
		{
			close(current->pipe_fd[1]);
			current->pipe_fd[1] = -1;
		}
		current = current->next;
	}
}
