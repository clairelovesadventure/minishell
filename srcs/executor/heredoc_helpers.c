/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_helpers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marrey <marrey@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 20:15:22 by marrey            #+#    #+#             */
/*   Updated: 2025/06/18 13:08:31 by marrey           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	child_readline_process(int pipe_fd[2])
{
	char	*line;

	close(pipe_fd[0]);
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	line = readline("> ");
	if (!line)
	{
		close(pipe_fd[1]);
		exit(1);
	}
	write(pipe_fd[1], line, ft_strlen(line));
	write(pipe_fd[1], "\n", 1);
	free(line);
	close(pipe_fd[1]);
	exit(0);
}

char	*handle_heredoc_fork_error(int pipe_fd[2])
{
	cleanup_pipe(pipe_fd);
	return (NULL);
}

char	*process_heredoc_read_result(t_shell *shell, char *buffer,
	ssize_t bytes_read)
{
	if (shell->heredoc_interrupted)
	{
		setup_readline(shell);
		return (NULL);
	}
	if (bytes_read <= 0)
		return (NULL);
	buffer[bytes_read] = '\0';
	return (process_heredoc_buffer(buffer, bytes_read));
}

char	*execute_heredoc_fork(t_shell *shell, int pipe_fd[2])
{
	pid_t	pid;
	char	buffer[1024];
	ssize_t	bytes_read;
	int		status;

	pid = fork();
	if (pid == -1)
		return (handle_heredoc_fork_error(pipe_fd));
	if (pid == 0)
		child_readline_process(pipe_fd);
	shell->readline_pid = pid;
	close(pipe_fd[1]);
	bytes_read = read(pipe_fd[0], buffer, sizeof(buffer) - 1);
	close(pipe_fd[0]);
	waitpid(pid, &status, 0);
	shell->readline_pid = 0;
	return (process_heredoc_read_result(shell, buffer, bytes_read));
}

int	setup_heredoc_pipe_and_signals(int pipe_fd[2])
{
	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		return (-1);
	}
	if (setup_heredoc_signals() == -1)
		return (-1);
	return (0);
}
