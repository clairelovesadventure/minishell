/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_cleanup.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 12:19:32 by shutan            #+#    #+#             */
/*   Updated: 2025/05/24 15:55:53 by marrey           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
