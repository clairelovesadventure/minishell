/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tolower.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 15:58:45 by shutan            #+#    #+#             */
/*   Updated: 2024/05/07 17:04:33 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <ctype.h>
#include <stdio.h>

int	ft_tolower(int c)
{
	if ((c >= 'A') && (c <= 'Z'))
	{
		c += 32;
	}
	return (c);
}

/*int	main (void)
{
	printf("%c", tolower('5'));
	printf("%c", ft_tolower('5'));
	return(0);
}*/
