/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_args.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marrey <marrey@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 03:05:00 by marrey            #+#    #+#             */
/*   Updated: 2025/07/20 03:07:53 by marrey           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	**allocate_new_args(int arg_count)
{
	char	**new_args;
	int		i;

	new_args = malloc(sizeof(char *) * (arg_count + 1));
	if (!new_args)
		return (NULL);
	i = 0;
	while (i <= arg_count)
	{
		new_args[i] = NULL;
		i++;
	}
	return (new_args);
}

static int	process_args_loop(t_cmd *cmd, char **new_args, t_shell *shell)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (cmd->args[i])
	{
		if (process_single_arg(new_args, &j, cmd->args[i], shell) == -1)
		{
			cleanup_args_on_error(new_args, j);
			return (-1);
		}
		free(cmd->args[i]);
		i++;
	}
	new_args[j] = NULL;
	return (0);
}

int	expand_args(t_cmd *cmd, t_shell *shell)
{
	char	**new_args;
	int		arg_count;

	if (!cmd->args)
		return (0);
	arg_count = 0;
	while (cmd->args[arg_count])
		arg_count++;
	new_args = allocate_new_args(arg_count);
	if (!new_args)
		return (-1);
	if (process_args_loop(cmd, new_args, shell) == -1)
	{
		free(new_args);
		return (-1);
	}
	free(cmd->args);
	cmd->args = new_args;
	return (0);
}
