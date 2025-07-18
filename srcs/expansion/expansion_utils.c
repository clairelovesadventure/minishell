/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marrey <marrey@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 23:52:00 by shutan            #+#    #+#             */
/*   Updated: 2025/07/20 01:33:05 by marrey           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*process_variable(char *str, int *i, t_env *env_list, int exit_status)
{
	char	*key;
	char	*value;
	int		start;

	(*i)++;
	start = *i;
	key = handle_special_vars(str, i);
	if (!key)
	{
		if (ft_isalpha(str[*i]) || str[*i] == '_')
			key = handle_normal_var(str, i, start);
		else
			return (ft_strdup("$"));
	}
	value = get_variable_value(key, env_list, exit_status);
	free(key);
	if (value)
		return (value);
	return (ft_strdup(""));
}

char	**perform_single_expansion(char *token, t_shell *shell)
{
	char	*vars_expanded_token;
	char	*quotes_removed_token;

	vars_expanded_token = expand_variables_in_str(token, shell);
	if (!vars_expanded_token)
		return (NULL);
	quotes_removed_token = remove_quotes_from_str(vars_expanded_token);
	free(vars_expanded_token);
	if (!quotes_removed_token)
		return (NULL);
	if (*quotes_removed_token == '\0')
	{
		free(quotes_removed_token);
		return (NULL);
	}
	return (create_single_result(quotes_removed_token));
}

int	process_single_arg(char **new_args, int *j, char *arg, t_shell *shell)
{
	char	**expanded;

	expanded = perform_single_expansion(arg, shell);
	if (!expanded)
		return (0);
	if (expanded[0] != NULL)
	{
		new_args[*j] = expanded[0];
		(*j)++;
	}
	free(expanded);
	return (0);
}
