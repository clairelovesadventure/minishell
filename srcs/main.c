/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 16:07:21 by shutan            #+#    #+#             */
/*   Updated: 2025/07/18 22:07:37 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	g_signal_status = 0;

void	cleanup_before_exit(t_shell *shell)
{
	// Free shell memory first (including environment variables)
	free_shell(shell);

	// Clear readline history and buffers to free memory
	cleanup_readline_completely();

	// Reset signal handlers to default
	reset_signals();

	// Restore terminal state
	restore_terminal_state();

	// Force flush of any remaining output
	fflush(stdout);
	fflush(stderr);

	// Close any remaining file descriptors (except stdin, stdout, stderr)
	for (int fd = 3; fd < 1024; fd++)
	{
		close(fd);
	}
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
	setup_readline();
	handle_input_loop(shell);
	last_status = shell->exit_status;
	// Final cleanup to ensure all command data is freed
	clean_current_command(shell);
	cleanup_before_exit(shell);
	return (last_status);
}
