/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_processing.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 17:15:00 by shutan            #+#    #+#             */
/*   Updated: 2025/07/18 21:56:19 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	process_non_interactive_input(t_shell *shell)
{
	char	buffer[4096];
	ssize_t	bytes_read;
	char	*input_copy;

	bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer) - 1);
	if (bytes_read > 0)
	{
		buffer[bytes_read] = '\0';
		input_copy = ft_strdup(buffer);
		if (!input_copy)
			return ;
		process_input_buffer(shell, input_copy);
		free(input_copy);
		// Ensure any remaining command data is cleaned up
		clean_current_command(shell);
	}
}

void	process_interactive_input(t_shell *shell)
{
	char	*current_input;

	while (1)
	{
		current_input = read_input();
		handle_signal_status(shell);
		if (!current_input)
		{
			// Print "exit" when EOF (Ctrl+D) is received, matching bash behavior
			if (isatty(STDIN_FILENO))
				ft_putendl_fd("exit", STDOUT_FILENO);
			break ;
		}
		if (current_input[0] != '\0')
		{
			shell->input = ft_strdup(current_input);
			free(current_input);
			if (shell->input)
			{
				add_history(shell->input);
				process_input(shell);
				clean_current_command(shell);
			}
		}
		else
			free(current_input);
	}
}

void	handle_input_loop(t_shell *shell)
{
	if (!isatty(STDIN_FILENO))
		process_non_interactive_input(shell);
	else
		process_interactive_input(shell);
}
