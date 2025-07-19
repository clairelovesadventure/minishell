/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_processing.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marrey <marrey@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 17:15:00 by shutan            #+#    #+#             */
/*   Updated: 2025/07/20 01:34:22 by marrey           ###   ########.fr       */
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
		clean_current_command(shell);
	}
}

void	process_interactive_input(t_shell *shell)
{
	char	*current_input;

	while (1)
	{
		current_input = read_input(shell);
		handle_signal_status(shell);
		if (!current_input)
		{
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
