/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 17:17:00 by shutan            #+#    #+#             */
/*   Updated: 2025/07/18 17:16:49 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	process_single_line(t_shell *shell, char *line)
{
	if (ft_strlen(line) > 0)
	{
		shell->input = ft_strdup(line);
		if (shell->input)
		{
			process_input(shell);
			clean_current_command(shell);
		}
	}
}

void	process_input_buffer(t_shell *shell, char *input_copy)
{
	char	*line;
	char	*newline_pos;

	line = input_copy;
	while (line && *line)
	{
		newline_pos = ft_strchr(line, '\n');
		if (newline_pos)
		{
			*newline_pos = '\0';
			newline_pos++;
		}
		process_single_line(shell, line);
		if (!newline_pos)
			break ;
		line = newline_pos;
	}
}

void	handle_signal_status(t_shell *shell)
{
	char	*exit_str;

	if (g_signal_status == 130)
	{
		shell->exit_status = 130;
		exit_str = ft_itoa(130);
		if (exit_str)
		{
			set_env_value(&(shell->env_list), "?", exit_str);
			free(exit_str);
		}
		g_signal_status = 0;
	}
}
