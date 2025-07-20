/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_vars.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marrey <marrey@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 19:19:15 by marrey            #+#    #+#             */
/*   Updated: 2025/05/30 18:29:16 by marrey           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*handle_special_vars(char *str, int *i)
{
	if (str[*i] == '?')
	{
		(*i)++;
		return (ft_strdup("?"));
	}
	else if (str[*i] == '$')
	{
		(*i)++;
		return (ft_strdup("$"));
	}
	return (NULL);
}

char	*handle_normal_var(char *str, int *i, int start)
{
	*i = skip_variable_name(str, *i);
	return (ft_substr(str, start, *i - start));
}

char	*get_variable_value(char *key, t_env *env_list, int exit_status)
{
	char	*value;
	char	*env_value;

	if (ft_strcmp(key, "?") == 0)
		value = ft_itoa(exit_status);
	else if (ft_strcmp(key, "$") == 0)
		value = ft_itoa(getpid());
	else
	{
		env_value = get_env_value(env_list, key);
		if (env_value)
			value = ft_strdup(env_value);
		else
			value = NULL;
	}
	return (value);
}

int	skip_variable_name(char *str, int i)
{
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	return (i);
}
