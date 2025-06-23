/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_check.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inabakka <inabakka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 13:04:09 by inabakka          #+#    #+#             */
/*   Updated: 2025/05/23 17:11:52 by inabakka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

long	ft_atol(const char *str)
{
	long	result;
	int		sign;

	result = 0;
	sign = 1;
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '-')
		sign = -1;
	if (*str == '-' || *str == '+')
		str++;
	while (*str >= '0' && *str <= '9')
	{
		result = result * 10 + (*str - '0');
		str++;
	}
	return (result * sign);
}

bool	check_input(char *str)
{
	int	i;

	i = 0;
	if (str[0] == '-' || str[0] == '+')
		i++;
	if (!str[i])
		return (false);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (false);
		i++;
	}
	if (ft_atol(str) > INT_MAX || ft_atol(str) < INT_MIN)
		return (false);
	return (true);
}

bool check_duplicates(t_stack_node *stack)
{
    t_stack_node *current;
    t_stack_node *check;
    
    if (!stack)
        return (false);
    
    current = stack;
    while (current)
    {
        check = current->next;
        while (check)
        {
            if (current->nbr == check->nbr)
                return (true);
            check = check->next;
        }
        current = current->next;
    }
    return (false);
}

void	error_exit(t_stack_node **a, t_stack_node **b,
	char **split, bool is_split)
{
	if (is_split && split)
		free_split(split);
	if (a)
		free_stack(a);
	if (b)
		free_stack(b);
	write(2, "Error\n", 6);
	exit(1);
}
