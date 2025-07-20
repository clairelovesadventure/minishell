/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 16:41:12 by shutan            #+#    #+#             */
/*   Updated: 2025/06/09 18:04:30 by marrey           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*ft_strjoin_free(char *s1, const char *s2)
{
	char	*result;

	if (!s1 || !s2)
		return (NULL);
	result = ft_strjoin(s1, s2);
	free(s1);
	return (result);
}

char	*ft_strjoin_char(char *s1, char c)
{
	char	*result;
	int		len;

	if (!s1)
		return (NULL);
	len = ft_strlen(s1);
	result = malloc(len + 2);
	if (!result)
		return (NULL);
	ft_memcpy(result, s1, len);
	result[len] = c;
	result[len + 1] = '\0';
	free(s1);
	return (result);
}
