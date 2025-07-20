/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 20:15:50 by shutan            #+#    #+#             */
/*   Updated: 2025/07/12 12:08:40 by marrey           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_shell	*init_shell(char **envp)
{
	t_shell	*shell;

	shell = (t_shell *)malloc(sizeof(t_shell));
	if (!shell)
		return (NULL);
	shell->env_list = init_env(envp);
	shell->cmd_list = NULL;
	shell->tokens = NULL;
	shell->input = NULL;
	shell->exit_status = 0;
	shell->readline_pid = 0;
	shell->heredoc_interrupted = 0;
	shell->term_saved = 0;
	return (shell);
}

void	clean_current_command(t_shell *shell)
{
	if (!shell)
		return ;
	if (shell->input)
	{
		free(shell->input);
		shell->input = NULL;
	}
	if (shell->tokens)
	{
		free_tokens(shell->tokens);
		shell->tokens = NULL;
	}
	if (shell->cmd_list)
	{
		free_cmds(shell->cmd_list);
		shell->cmd_list = NULL;
	}
}

void	free_shell(t_shell *shell)
{
	if (!shell)
		return ;
	clean_current_command(shell);
	if (shell->env_list)
	{
		free_env(shell->env_list);
		shell->env_list = NULL;
	}
	free(shell);
}
