/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 14:38:41 by shutan            #+#    #+#             */
/*   Updated: 2024/05/14 20:38:08 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

void	ft_putstr_fd(char *s, int fd)
{
	int	len;

	len = 0;
	while (s[len])
	{
		write(fd, &s[len], 1);
		len++;
	}
}

/* int	main(void)
{
	char	s[] = "Hallo";

	ft_putstr_fd(s, 1);
	return (0);
} */