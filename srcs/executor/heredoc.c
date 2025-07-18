/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:30:00 by shutan            #+#    #+#             */
/*   Updated: 2025/07/18 17:18:16 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

volatile pid_t	g_readline_pid = 0;
volatile int		g_heredoc_interrupted = 0;

static int	child_readline_process(int pipe_fd[2])
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

static char	*handle_heredoc_fork_error(int pipe_fd[2])
{
	cleanup_pipe(pipe_fd);
	return (NULL);
}

static char	*read_heredoc_line(void)
{
	int		pipe_fd[2];
	pid_t	pid;
	char	buffer[1024];
	ssize_t	bytes_read;
	int		status;

	if (g_heredoc_interrupted)
		return (NULL);
	if (pipe(pipe_fd) == -1)
		return (NULL);
	pid = fork();
	if (pid == -1)
		return (handle_heredoc_fork_error(pipe_fd));
	if (pid == 0)
		child_readline_process(pipe_fd);
	g_readline_pid = pid;
	close(pipe_fd[1]);
	bytes_read = read(pipe_fd[0], buffer, sizeof(buffer) - 1);
	close(pipe_fd[0]);
	waitpid(pid, &status, 0);
	g_readline_pid = 0;
	if (g_heredoc_interrupted)
	{
		setup_readline();
		return (NULL);
	}
	if (bytes_read <= 0)
		return (NULL);
	buffer[bytes_read] = '\0';
	return (process_heredoc_buffer(buffer, bytes_read));
}

int	handle_heredoc(char *delimiter)
{
	int		pipe_fd[2];
	char	*line;

	g_heredoc_interrupted = 0;
	g_readline_pid = 0;
	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		return (-1);
	}
	if (setup_heredoc_signals() == -1)
		return (-1);
	while (1)
	{
		line = read_heredoc_line();
		if (!line)
			break ;
		if (process_heredoc_line(pipe_fd, line, delimiter))
			break ;
	}
	restore_heredoc_signals();
	close(pipe_fd[1]);
	if (g_heredoc_interrupted)
		return (handle_heredoc_interruption(pipe_fd));
	return (pipe_fd[0]);
}
