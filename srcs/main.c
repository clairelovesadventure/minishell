/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 20:27:41 by shutan            #+#    #+#             */
/*   Updated: 2025/06/23 10:33:30 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int		g_signal_status = 0;

void	cleanup_before_exit(t_shell *shell)
{
	restore_terminal_state(shell);
	cleanup_readline_completely();
	reset_signals();
	free_shell(shell);
	fflush(stdout);
	fflush(stderr);
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	*shell;
	int		last_status;

	(void)argc;
	(void)argv;
	shell = init_shell(envp);
	if (!shell)
		return (1);
	setup_signals();
	setup_readline(shell);
	handle_input_loop(shell);
	last_status = shell->exit_status;
	clean_current_command(shell);
	cleanup_before_exit(shell);
	return (last_status);
}
