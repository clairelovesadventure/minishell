/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_expansion.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marrey <marrey@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 13:34:18 by marrey            #+#    #+#             */
/*   Updated: 2025/07/03 20:38:28 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*expand_variables_in_str(char *str, t_shell *shell)
{
	if (!str || !shell)
		return (NULL);
	return (expand_variables(str, shell->env_list, shell->exit_status));
}
