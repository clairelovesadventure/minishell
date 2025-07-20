/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marrey <marrey@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 21:45:30 by marrey            #+#    #+#             */
/*   Updated: 2025/07/20 03:05:22 by marrey           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* Function declaration from expansion_args.c */
int	expand_args(t_cmd *cmd, t_shell *shell);

static int	process_redirect_expansion(t_redirect *current, t_shell *shell)
{
	char	**expanded;

	expanded = perform_single_expansion(current->file, shell);
	if (!expanded)
		return (0);
	if (expanded[0] == NULL || expanded[1] != NULL || *expanded[0] == '\0')
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(current->file, STDERR_FILENO);
		ft_putstr_fd(": ambiguous redirect\n", STDERR_FILENO);
		free_str_array(expanded);
		shell->exit_status = 1;
		return (-1);
	}
	free(current->file);
	current->file = expanded[0];
	free(expanded);
	return (1);
}

static int	expand_redirects(t_cmd *cmd, t_shell *shell)
{
	t_redirect	*current;
	int			result;

	current = cmd->redirects;
	while (current)
	{
		result = process_redirect_expansion(current, shell);
		if (result == -1)
			return (-1);
		current = current->next;
	}
	return (0);
}

int	expand_command(t_cmd *cmd, t_shell *shell)
{
	if (!cmd)
		return (0);
	if (expand_args(cmd, shell) != 0)
		return (-1);
	if (expand_redirects(cmd, shell) != 0)
	{
		if (cmd->args)
		{
			free_str_array(cmd->args);
			cmd->args = NULL;
		}
		return (-1);
	}
	return (0);
}
