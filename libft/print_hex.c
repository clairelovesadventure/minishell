/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_hex.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42berlin.de>         +#+  +:+       +#+       */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 11:05:30 by shutan            #+#    #+#             */
/*   Updated: 2024/05/04 13:00:10 by shutan           ###   ########.fr      */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	print_hex(unsigned int num, char format)
{
	int		count;
	char	*hex;

	count = 0;
	if (num == 0)
		return (write(1, "0", 1));
	if (format == 'x')
		hex = "0123456789abcdef";
	else
		hex = "0123456789ABCDEF";
	if (num >= 16)
		count += print_hex(num / 16, format);
	count += write(1, &hex[num % 16], 1);
	return (count);
}
