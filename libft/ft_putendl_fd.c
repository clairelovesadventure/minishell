/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putendl_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/05 09:04:24 by shutan            #+#    #+#             */
/*   Updated: 2024/05/12 14:26:51 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

void	ft_putendl_fd(char *s, int fd)
{
	int	len;

	len = 0;
	while (s[len])
	{
		write(fd, &s[len], 1);
		len++;
	}
	write(fd, "\n", 1);
}

/* int	main(void)
{
	char	s[] = "Hallo";

	ft_putendl_fd(s, 1);
	return (0);
} */