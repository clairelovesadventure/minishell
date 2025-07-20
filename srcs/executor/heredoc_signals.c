/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_signals.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 11:39:13 by shutan            #+#    #+#             */
/*   Updated: 2025/07/20 16:50:44 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static t_shell	*g_current_shell = NULL;

void	set_heredoc_shell(t_shell *shell)
{
	g_current_shell = shell;
}

void	heredoc_sigint_handler(int sig)
{
	(void)sig;
	if (g_current_shell)
	{
		g_current_shell->heredoc_interrupted = 1;
		if (g_current_shell->readline_pid > 0)
		{
			kill(g_current_shell->readline_pid, SIGINT);
			g_current_shell->readline_pid = 0;
		}
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
	if (g_current_shell && g_current_shell->heredoc_interrupted)
		setup_readline(g_current_shell);
}
