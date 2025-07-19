/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_preprocess.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 16:26:50 by shutan            #+#    #+#             */
/*   Updated: 2025/07/18 16:26:50 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	cleanup_on_error(int fd, int temp_fd, char *temp_file)
{
	close(fd);
	if (temp_fd != -1)
		close(temp_fd);
	if (temp_file)
	{
		unlink(temp_file);
		free(temp_file);
	}
	return (-1);
}

static int	copy_heredoc_content(int fd, int temp_fd, char *temp_file)
{
	char	buffer[1024];
	ssize_t	bytes_read;

	while (1)
	{
		bytes_read = read(fd, buffer, sizeof(buffer));
		if (bytes_read <= 0)
			break ;
		if (write(temp_fd, buffer, bytes_read) != bytes_read)
			return (cleanup_on_error(fd, temp_fd, temp_file));
	}
	return (0);
}

static int	create_temp_file(int fd, char **temp_file, int *temp_fd)
{
	*temp_file = ft_strdup("/tmp/heredoc_XXXXXX");
	if (!*temp_file)
		return (cleanup_on_error(fd, -1, NULL));
	*temp_fd = mkstemp(*temp_file);
	if (*temp_fd == -1)
		return (cleanup_on_error(fd, -1, *temp_file));
	return (0);
}

int	preprocess_heredoc(t_redirect *redirect, t_shell *shell)
{
	int		fd;
	char	*temp_file;
	int		temp_fd;

	fd = handle_heredoc(redirect->file, shell);
	if (fd == -1)
		return (-1);
	if (create_temp_file(fd, &temp_file, &temp_fd) == -1)
		return (-1);
	if (copy_heredoc_content(fd, temp_fd, temp_file) == -1)
		return (-1);
	close(fd);
	close(temp_fd);
	free(redirect->file);
	redirect->file = temp_file;
	redirect->type = T_REDIR_IN;
	return (0);
}

void	cleanup_temp_files(t_cmd *cmd_list)
{
	t_cmd		*current;
	t_redirect	*redirect;

	current = cmd_list;
	while (current)
	{
		redirect = current->redirects;
		while (redirect)
		{
			if (redirect->type == T_REDIR_IN && redirect->file
				&& ft_strncmp(redirect->file, "/tmp/heredoc_", 13) == 0)
			{
				unlink(redirect->file);
			}
			redirect = redirect->next;
		}
		current = current->next;
	}
}

int	preprocess_heredocs_in_cmd(t_cmd *cmd, t_shell *shell)
{
	t_redirect	*redirect;

	redirect = cmd->redirects;
	while (redirect)
	{
		if (redirect->type == T_HEREDOC)
		{
			if (preprocess_heredoc(redirect, shell) == -1)
				return (-1);
		}
		redirect = redirect->next;
	}
	return (0);
}

int	preprocess_all_heredocs(t_cmd *cmd_list, t_shell *shell)
{
	t_cmd	*current;

	current = cmd_list;
	while (current)
	{
		if (preprocess_heredocs_in_cmd(current, shell) == -1)
			return (-1);
		current = current->next;
	}
	return (0);
}
