/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   filename_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marrey <marrey@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 11:37:02 by marrey            #+#    #+#             */
/*   Updated: 2025/07/15 10:30:14 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*strip_surrounding_quotes(const char *str)
{
	int	len;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	if (len >= 2)
	{
		if ((str[0] == '\'' && str[len - 1] == '\'')
			|| (str[0] == '"' && str[len - 1] == '"'))
		{
			return (ft_substr(str, 1, len - 2));
		}
	}
	return (ft_strdup(str));
}

char	*process_filename_quotes(const char *raw_filename)
{
	return (strip_surrounding_quotes(raw_filename));
}
