/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_ptr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 09:55:17 by shutan         #+#    #+#             */
/*   Updated: 2024/04/27 17:30:53 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	print_ptr(unsigned long int pa)
{
	write(1, "0x", 2);
	return (print_hex(pa, 'x') + 2);
}

// int	main(void)
// {
// 	int	*a;
// 	int	A;
// 	A = 33;
// 	a = &A;
// 	printf("\n%d", putptr((unsigned long)a));
// 	printf("\n%p", a);
// 	return (0);
// }