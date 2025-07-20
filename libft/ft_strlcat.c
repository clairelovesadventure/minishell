/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 13:30:49 by shutan            #+#    #+#             */
/*   Updated: 2024/05/06 17:16:15 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stddef.h>

size_t	ft_strlcat(char *dest, const char *src, size_t size)
{
	unsigned int	j;
	unsigned int	i;

	i = ft_strlen(dest);
	j = 0;
	if (size < 1)
		return (size + ft_strlen(src));
	while (src[j] != '\0' && i + j < size - 1)
	{
		dest[i + j] = src[j];
		j++;
	}
	dest[i + j] = '\0';
	if (size <= i)
		return (ft_strlen(src) + size);
	else
		return (i + ft_strlen(src));
}

/*int	main(void)
{
	char	dest[10] = "Hallo,";
	char	src[] = "world!";
	printf("%u", ft_strlcat(dest, src, sizeof(dest)));
}*/
