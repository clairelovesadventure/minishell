/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   small_sort.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marrey <marrey@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 13:05:02 by inabakka          #+#    #+#             */
/*   Updated: 2025/06/23 12:34:08 by marrey           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	sort_three(t_stack_node **a)
{
	int	first, second, third;
	
	if (!a || !*a || !(*a)->next || !(*a)->next->next)
		return;
	
	first = (*a)->nbr;
	second = (*a)->next->nbr;
	third = (*a)->next->next->nbr;
	
	// All 6 possible cases for 3 elements
	if (first < second && second < third) // 1 2 3 - already sorted
		return;
	else if (first < second && second > third && first < third) // 1 3 2
		{
			rra(a, true);
			sa(a, true);
		}
	else if (first > second && second < third && first < third) // 2 1 3
		sa(a, true);
	else if (first < second && second > third && first > third) // 3 1 2
		rra(a, true);
	else if (first > second && second > third) // 3 2 1
		{
			ra(a, true);
			sa(a, true);
		}
	else if (first > second && second < third && first > third) // 2 3 1
		ra(a, true);
}

// Old version - had issues with logic
// void sort_three(t_stack_node **a)
// {
//     if (!a || !*a || !(*a)->next || !(*a)->next->next)
//         return;

//     int first = (*a)->nbr;               // First node's value
//     int second = (*a)->next->nbr;        // Second node's value
//     int third = (*a)->next->next->nbr;   // Third node's value

//     if (first > second && second < third && first < third)
//         sa(a, true);
//     else if (first > second && second > third)
//     {
//         sa(a, true);
//         rra(a, true);
//     }
//     else if (first > second && second < third && first > third)
//         ra(a, true);
//     else if (first < second && second > third && first < third)
//     {
//         sa(a, true);
//         ra(a, true);
//     }
//     else if (first < second && second > third && first > third)
//         rra(a, true);
// }