/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marrey <marrey@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:30:00 by shutan            #+#    #+#             */
/*   Updated: 2025/07/20 01:06:29 by marrey           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

static char	*read_heredoc_line(t_shell *shell)
{
	int		pipe_fd[2];
	pid_t	pid;
	char	buffer[1024];
	ssize_t	bytes_read;
	int		status;

	if (shell->heredoc_interrupted)
		return (NULL);
	if (pipe(pipe_fd) == -1)
		return (NULL);
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

int	handle_heredoc(char *delimiter, t_shell *shell)
{
	int		pipe_fd[2];
	char	*line;

	shell->heredoc_interrupted = 0;
	shell->readline_pid = 0;
	set_heredoc_shell(shell);
	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		return (-1);
	}
	if (setup_heredoc_signals() == -1)
		return (-1);
	while (1)
	{
		line = read_heredoc_line(shell);
		if (!line)
			break ;
		if (process_heredoc_line(pipe_fd, line, delimiter))
			break ;
	}
	restore_heredoc_signals();
	close(pipe_fd[1]);
	if (shell->heredoc_interrupted)
		return (handle_heredoc_interruption(pipe_fd));
	return (pipe_fd[0]);
}
