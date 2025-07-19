/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marrey <marrey@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 16:07:21 by shutan            #+#    #+#             */
/*   Updated: 2025/07/20 01:01:32 by marrey           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	g_signal_status = 0;

void	cleanup_before_exit(t_shell *shell)
{
	free_shell(shell);
	cleanup_readline_completely();
	reset_signals();
	restore_terminal_state(shell);
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
