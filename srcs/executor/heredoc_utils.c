/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 20:09:22 by shutan            #+#    #+#             */
/*   Updated: 2025/07/15 15:55:19 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	cleanup_pipe(int pipe_fd[2])
{
	close(pipe_fd[0]);
	close(pipe_fd[1]);
}

char	*process_heredoc_buffer(char *buffer, ssize_t bytes_read)
{
	while (bytes_read > 0 && (buffer[bytes_read - 1] == '\n'
			|| buffer[bytes_read - 1] == '\r'))
		buffer[--bytes_read] = '\0';
	return (ft_strdup(buffer));
}

int	handle_heredoc_interruption(int pipe_fd[2])
{
	close(pipe_fd[0]);
	g_signal_status = 130;
	return (-1);
}
