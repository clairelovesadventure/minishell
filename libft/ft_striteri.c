/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/28 17:52:29 by shutan            #+#    #+#             */
/*   Updated: 2024/05/04 16:29:00 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>

/* void	ft_print(unsigned int n, char *c)
{
	printf("The %d th charactor's address is %p \n", n, c);
} */

void	ft_striteri(char *s, void (*f)(unsigned int, char *))
{
	int	i;

	i = 0;
	while (s[i] != '\0')
	{
		f(i, &s[i]);
		i++;
	}
}

/* int	main(void)
{
	char	s[] = "Hallo";
	ft_striteri(s, ft_print);
	return (0);
} */