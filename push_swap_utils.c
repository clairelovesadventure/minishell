/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_swap_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inabakka <inabakka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 16:52:36 by inabakka          #+#    #+#             */
/*   Updated: 2025/05/23 16:55:21 by inabakka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

t_stack_node	*find_last_node(t_stack_node *stack)
{
	if (NULL == stack)
		return (NULL);
	while (stack->next)
		stack = stack->next;
	return (stack);
}

int	stack_len(t_stack_node *stack) //Define a function that calculates and returns the length of a stack
{
	int	count; //To store the node count

	if (!stack) 
		return (0);
	count = 0;
	while (stack) //Loop until the end of the stack is reached
	{
		stack = stack->next; //Move to the next node
		count++;
	}
	return (count);
}

bool stack_sorted(t_stack_node *stack)
{
    t_stack_node *current;

    if (!stack)  // Empty stack is considered sorted
        return (true);

    current = stack;  // Start at the top node
    while (current->next)  // While not at the last node
    {
        if (current->nbr > current->next->nbr)
            return (false);  // Found unsorted pair
        current = current->next;  // Move to next node
    }
    return (true);  // All elements are in order
}

t_stack_node *find_smallest(t_stack_node *stack)
{
    long smallest;
    t_stack_node *smallest_node;

    if (!stack)
        return (NULL);

    smallest = LONG_MAX;
    smallest_node = NULL;

    while (stack)
    {
        if (stack->nbr < smallest)
        {
            smallest = stack->nbr;
            smallest_node = stack;
        }
        stack = stack->next;
    }
    return (smallest_node);
}

t_stack_node			*find_max_node(t_stack_node *stack)
{
	long			max; //To store the biggest value so far
	t_stack_node	*max_node; //To store a pointer that points to the biggest number

	if (!stack)
		return (NULL);
	max = LONG_MIN; //Assign to the biggest value so far, the max long integer
	while (stack) //Loop until the end of the stack is reached
	{
		if (stack->nbr > max) //Check if the current node value is smaller than the biggest so far
		{
			max = stack->nbr; //If so, update the biggest number so far
			max_node = stack; //Set the pointer to point to the node with the biggest number so far
		}
		stack = stack->next; //Move to the next node for processing
	}
	return (max_node);
}