/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marrey <marrey@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 09:00:45 by marrey            #+#    #+#             */
/*   Updated: 2025/05/26 17:25:11 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* External function declarations from heredoc_helpers.c */
char	*process_heredoc_read_result(t_shell *shell, char *buffer,
			ssize_t bytes_read);
char	*execute_heredoc_fork(t_shell *shell, int pipe_fd[2]);
int		setup_heredoc_pipe_and_signals(int pipe_fd[2]);
int		child_readline_process(int pipe_fd[2]);
char	*handle_heredoc_fork_error(int pipe_fd[2]);

static char	*read_heredoc_line(t_shell *shell)
{
	int	pipe_fd[2];

	if (shell->heredoc_interrupted)
		return (NULL);
	if (pipe(pipe_fd) == -1)
		return (NULL);
	return (execute_heredoc_fork(shell, pipe_fd));
}

static void	process_heredoc_loop(int pipe_fd[2], char *delimiter,
	t_shell *shell)
{
	char	*line;

	while (1)
	{
		line = read_heredoc_line(shell);
		if (!line)
			break ;
		if (process_heredoc_line(pipe_fd, line, delimiter, shell))
			break ;
	}
}

int	handle_heredoc(char *delimiter, t_shell *shell)
{
	int	pipe_fd[2];

	shell->heredoc_interrupted = 0;
	shell->readline_pid = 0;
	set_heredoc_shell(shell);
	if (setup_heredoc_pipe_and_signals(pipe_fd) == -1)
		return (-1);
	process_heredoc_loop(pipe_fd, delimiter, shell);
	restore_heredoc_signals();
	close(pipe_fd[1]);
	if (shell->heredoc_interrupted)
		return (handle_heredoc_interruption(pipe_fd));
	return (pipe_fd[0]);
}
