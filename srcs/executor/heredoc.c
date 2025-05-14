/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marrey <marrey@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:30:00 by shutan            #+#    #+#             */
/*   Updated: 2025/05/13 19:45:35 by marrey           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
** Helper function for simple unquoting.
** IMPORTANT: This is a simplified unquoting mechanism.
** A full solution requires the parser to handle quote removal on the delimiter
** and also track if the original delimiter was quoted to control expansion.
*/
static char	*simple_unquote(const char *str)
{
	int		len;
	char	*unquoted_str;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	if (len >= 2)
	{
		if ((str[0] == '"' && str[len - 1] == '"') || \
			(str[0] == '\'' && str[len - 1] == '\''))
		{
			unquoted_str = ft_substr(str, 1, len - 2);
			if (!unquoted_str)
				perror("malloc in ft_substr for simple_unquote");
			return (unquoted_str);
		}
	}
	/* Return a copy if no simple unquoting is needed */
	unquoted_str = ft_strdup(str);
	if (!unquoted_str)
		perror("malloc in ft_strdup for simple_unquote");
	return (unquoted_str);
}

/*
** Processes heredoc redirection.
** Reads input until the specified delimiter is encountered.
** Returns a file descriptor containing the read content.
*/
int	handle_heredoc(char *delimiter)
{
	int		pipe_fd[2];
	char	*line;
	char	*cleaned_delimiter;
	char	*trimmed_line;

	if (!delimiter)
		return (-1);
	cleaned_delimiter = simple_unquote(delimiter);
	if (!cleaned_delimiter)
	{
		/* perror was called in simple_unquote if malloc failed */
		return (-1);
	}
	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		free(cleaned_delimiter);
		return (-1);
	}
	/* Disable interrupt signal handling to avoid interference with readline */
	signal(SIGINT, SIG_DFL);
	while (1)
	{
		/* Read input using readline (not saved to history) */
		line = readline("> ");
		/* If read fails or EOF is encountered, break the loop */
		if (!line)
		{
			/* EOF (Ctrl+D) */
			/* TODO: Add warning: "warning: here-document ... */
			/* ... delimited by end-of-file" */
			break ;
		}
		trimmed_line = ft_strtrim(line, " \\t\\n\\v\\f\\r");
		if (!trimmed_line)
		{
			free(line);
			/* TODO: Handle allocation failure for trimmed_line robustly */
			break ;
		}
		/* Compare trimmed line with the cleaned delimiter */
		if (ft_strcmp(trimmed_line, cleaned_delimiter) == 0)
		{
			free(line);
			free(trimmed_line);
			break ;
		}
		/* Write the read line to the pipe */
		write(pipe_fd[1], line, ft_strlen(line));
		write(pipe_fd[1], "\n", 1);
		fprintf(stderr, "[MSHELL_DEBUG_HEREDOC_WRITE] Wrote to heredoc pipe: [%s\\n]\n", line);
		fflush(stderr);
		free(trimmed_line);
		free(line);
	}
	free(cleaned_delimiter);
	/* Restore signal handling */
	setup_signals();
	/* Close the write end of the pipe and return the read end */
	close(pipe_fd[1]);
	return (pipe_fd[0]);
} 