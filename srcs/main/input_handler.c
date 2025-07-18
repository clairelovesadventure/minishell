/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 00:00:00 by shutan            #+#    #+#             */
/*   Updated: 2025/07/18 21:39:45 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <readline/readline.h>
#include <unistd.h>

void	handle_sigint_prompt(void)
{
	if (g_signal_status == 130)
	{
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		rl_redisplay();
	}
}

static char	*get_prompt_string(void)
{
	if (isatty(STDIN_FILENO))
		return ("minishell> ");
	return ("");
}

static char	*read_interactive_input(void)
{
	char	*prompt_str;
	char	*input;

	prompt_str = get_prompt_string();
	input = readline(prompt_str);
	if (!input)
	{
		// EOF received (Ctrl+D) - ensure terminal is properly restored
		restore_terminal_state();
		return (NULL);
	}
	return (input);
}

static char	*read_non_interactive_input(void)
{
	char	buffer[1024];
	ssize_t	bytes_read;
	int		i;
	char	*input;

	bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer) - 1);
	if (bytes_read <= 0)
		return (NULL);
	buffer[bytes_read] = '\0';
	i = bytes_read - 1;
	while (i >= 0 && (buffer[i] == '\n' || buffer[i] == '\r'))
		buffer[i--] = '\0';
	input = ft_strdup(buffer);
	return (input);
}

char	*read_input(void)
{
	if (isatty(STDIN_FILENO))
		return (read_interactive_input());
	else
		return (read_non_interactive_input());
}
