/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marrey <marrey@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 20:20:06 by marrey            #+#    #+#             */
/*   Updated: 2025/06/09 17:43:19 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "exit_utils.h"

int	is_numeric_argument(const char *str)
{
	char	*clean_str;

	if (!str || !*str)
		return (0);
	clean_str = clean_quoted_str(str);
	if (clean_str)
	{
		str = clean_str;
		if (!check_numeric_chars(str))
		{
			free(clean_str);
			return (0);
		}
		free(clean_str);
		return (1);
	}
	return (check_numeric_chars(str));
}

long	ft_atol(const char *str)
{
	long	result;
	int		sign;

	result = 0;
	sign = 1;
	while (*str == ' ' || (*str >= '\t' && *str <= '\r'))
		str++;
	if (*str == '+' || *str == '-')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (*str && ft_isdigit(*str))
	{
		result = result * 10 + (*str - '0');
		str++;
	}
	return (result * sign);
}

char	*get_key_from_str(const char *str)
{
	int		i;
	char	*key;

	i = 0;
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	key = ft_substr(str, 0, i);
	return (key);
}

int	handle_exit_args(char **args, long *exit_code)
{
	char	*clean_arg;
	int		result;

	clean_arg = NULL;
	result = validate_exit_args(args, &clean_arg);
	if (result)
		return (result);
	if (args[1])
	{
		if (clean_arg)
		{
			*exit_code = ft_atol(clean_arg);
			free(clean_arg);
		}
		else
			*exit_code = ft_atol(args[1]);
		if (*exit_code < 0 || *exit_code > 255)
			*exit_code %= 256;
		if (*exit_code < 0)
			*exit_code += 256;
	}
	else
		*exit_code = 0;
	return (0);
}
