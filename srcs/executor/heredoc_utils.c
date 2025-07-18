/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 17:16:00 by shutan            #+#    #+#             */
/*   Updated: 2025/07/18 17:14:53 by shutan           ###   ########.fr       */
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

int	process_heredoc_line(int pipe_fd[2], char *line, char *delimiter)
{
	if (ft_strcmp(line, delimiter) == 0)
	{
		free(line);
		return (1);
	}
	write(pipe_fd[1], line, ft_strlen(line));
	write(pipe_fd[1], "\n", 1);
	free(line);
	return (0);
}

int	handle_heredoc_interruption(int pipe_fd[2])
{
	close(pipe_fd[0]);
	g_signal_status = 130;
	return (-1);
}
