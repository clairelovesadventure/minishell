/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_signals.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 16:25:30 by shutan            #+#    #+#             */
/*   Updated: 2025/07/18 16:25:30 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	heredoc_sigint_handler(int sig)
{
	(void)sig;
	g_heredoc_interrupted = 1;
	if (g_readline_pid > 0)
	{
		kill(g_readline_pid, SIGINT);
		g_readline_pid = 0;
	}
	write(STDOUT_FILENO, "\n", 1);
}

int	setup_heredoc_signals(void)
{
	struct sigaction	sa;

	sa.sa_handler = heredoc_sigint_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	if (sigaction(SIGINT, &sa, NULL) == -1)
		return (-1);
	signal(SIGQUIT, SIG_IGN);
	return (0);
}

void	restore_heredoc_signals(void)
{
	setup_signals();
	if (g_heredoc_interrupted)
		setup_readline();
}
