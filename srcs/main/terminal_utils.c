/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminal_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 19:02:14 by shutan            #+#    #+#             */
/*   Updated: 2025/05/25 13:48:02 by marrey           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <termios.h>

/* Function declarations from readline_utils.c */
void	clear_readline_buffers(void);
void	cleanup_readline_completely(void);

static int	save_terminal_state(t_shell *shell)
{
	if (isatty(STDIN_FILENO))
	{
		if (tcgetattr(STDIN_FILENO, &shell->original_term) == -1)
			return (-1);
		shell->term_saved = 1;
	}
	return (0);
}

static int	setup_terminal_for_readline(t_shell *shell)
{
	struct termios	term;

	if (!isatty(STDIN_FILENO))
		return (0);
	if (!shell->term_saved)
	{
		if (save_terminal_state(shell) == -1)
			return (-1);
	}
	term = shell->original_term;
	term.c_lflag &= ~ECHOCTL;
	if (tcsetattr(STDIN_FILENO, TCSANOW, &term) == -1)
		return (-1);
	return (0);
}

void	setup_readline(t_shell *shell)
{
	if (setup_terminal_for_readline(shell) == -1)
		return ;
	rl_catch_signals = 0;
	rl_completion_append_character = ' ';
	rl_completion_query_items = 100;
}

void	restore_terminal_state(t_shell *shell)
{
	if (shell->term_saved && isatty(STDIN_FILENO))
	{
		tcsetattr(STDIN_FILENO, TCSANOW, &shell->original_term);
	}
}
