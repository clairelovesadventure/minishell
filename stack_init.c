/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marrey <marrey@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 13:05:08 by inabakka          #+#    #+#             */
/*   Updated: 2025/06/23 12:36:53 by marrey           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void free_stack(t_stack_node **stack)
{
    if (!stack || !*stack)
        return;
    
    t_stack_node *current = *stack;  // Start from the head node
    t_stack_node *tmp;
    
    while (current)
    {
        tmp = current->next;
        free(current);
        current = tmp;
    }
    
    *stack = NULL;  // Set the stack pointer to NULL
}

void	free_split(char **split)
{
	int	i;

	i = 0;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}

void append_node(t_stack_node **stack, int n)
{
    if (!stack)
        return;
    
    t_stack_node *node = malloc(sizeof(t_stack_node));  // Allocate node, not stack
    if (!node)
        return;
    
    node->nbr = n;       // Use 'nbr' instead of 'value'
    node->next = NULL;
    node->prev = NULL;
    
    if (!*stack)
    {
        *stack = node;   // Just assign the node if stack is empty
        return;
    }
    
    t_stack_node *last = *stack;  // Start from the head
    while (last->next)            // Traverse to the end
        last = last->next;
    
    last->next = node;   // Link new node
    node->prev = last;
}

int	stack_init_a(t_stack_node **a, char **argv, bool is_split)
{
	long	n;
	int		i;

	i = 0;
	while (argv[i])
	{
		if (!check_input(argv[i]))
			error_exit(a, NULL, argv, is_split);
		n = ft_atol(argv[i]);
		append_node(a, (int)n);
		i++;
	}
	if (check_duplicates(*a))
		error_exit(a, NULL, argv, is_split);
	return (1);
}

t_stack_node	*return_cheapest(t_stack_node *stack)
{
	if (stack == NULL)
		return (NULL);
	while (stack)
	{
		if (stack->cheapest)
			return (stack);
		stack = stack->next;
	}
	return (NULL);
}

void	prep_for_push(t_stack_node **stack,
						t_stack_node *top_node,
						char stack_name) //Define a function that moves the required node to the top of the stack
{
	while (*stack != top_node) //Check if the required node is not already the first node
	{
		if (stack_name == 'a') //If not, and it is stack `a`, execute the following
		{
			if (top_node->above_median)
				ra(stack, true);
			else
				rra(stack, true);
		}
		else if (stack_name == 'b') //If not, and it is stack `b`, execute the following
		{
			if (top_node->above_median)
				rb(stack, true);
			else
				rrb(stack, true);
		}	
	}
}