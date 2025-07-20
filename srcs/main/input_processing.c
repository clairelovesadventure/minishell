/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_processing.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marrey <marrey@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 16:02:00 by marrey            #+#    #+#             */
/*   Updated: 2025/04/14 12:20:34 by shutan           ###   ########.fr       */
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

static void	process_valid_input(t_shell *shell, char *current_input)
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

static int	handle_input_result(t_shell *shell, char *current_input)
{
	if (!current_input)
	{
		if (isatty(STDIN_FILENO))
			ft_putendl_fd("exit", STDOUT_FILENO);
		return (0);
	}
	if (current_input[0] != '\0')
		process_valid_input(shell, current_input);
	else
		free(current_input);
	return (1);
}

void	process_interactive_input(t_shell *shell)
{
	char	*current_input;

	while (1)
	{
		current_input = read_input(shell);
		handle_signal_status(shell);
		if (!handle_input_result(shell, current_input))
			break ;
	}
}

void	handle_input_loop(t_shell *shell)
{
	if (!isatty(STDIN_FILENO))
		process_non_interactive_input(shell);
	else
		process_interactive_input(shell);
}
