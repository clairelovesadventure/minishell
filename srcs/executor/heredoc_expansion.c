/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_expansion.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 08:55:00 by shutan            #+#    #+#             */
/*   Updated: 2025/07/20 08:52:43 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	is_delimiter_quoted(const char *delimiter)
{
	int	len;

	if (!delimiter)
		return (0);
	len = ft_strlen(delimiter);
	if (len >= 2)
	{
		if ((delimiter[0] == '\'' && delimiter[len - 1] == '\'')
			|| (delimiter[0] == '"' && delimiter[len - 1] == '"'))
			return (1);
	}
	return (0);
}

static char	*get_clean_delimiter(const char *delimiter)
{
	if (is_delimiter_quoted(delimiter))
		return (process_filename_quotes(delimiter));
	return (ft_strdup(delimiter));
}

static void	write_expanded_line(int pipe_fd[2], char *line,
	int should_expand, t_shell *shell)
{
	char	*expanded_line;

	if (should_expand && shell)
	{
		expanded_line = expand_variables_in_str(line, shell);
		if (expanded_line)
		{
			write(pipe_fd[1], expanded_line, ft_strlen(expanded_line));
			free(expanded_line);
		}
		else
			write(pipe_fd[1], line, ft_strlen(line));
	}
	else
		write(pipe_fd[1], line, ft_strlen(line));
}

int	process_heredoc_line(int pipe_fd[2], char *line,
	char *delimiter, t_shell *shell)
{
	char	*clean_delimiter;
	int		should_expand;

	clean_delimiter = get_clean_delimiter(delimiter);
	if (!clean_delimiter)
	{
		free(line);
		return (1);
	}
	if (ft_strcmp(line, clean_delimiter) == 0)
	{
		free(line);
		free(clean_delimiter);
		return (1);
	}
	should_expand = !is_delimiter_quoted(delimiter);
	write_expanded_line(pipe_fd, line, should_expand, shell);
	write(pipe_fd[1], "\n", 1);
	free(line);
	free(clean_delimiter);
	return (0);
}
