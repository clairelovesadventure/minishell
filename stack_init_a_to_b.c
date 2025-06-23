#include "push_swap.h"

/*
 * Set the current position of every node 
 * in the current state-configuration
*/
void	set_current_position(t_stack_node *stack)
{
	int	i;
	int	median;

	i = 0;
	if (NULL == stack)
		return ;
	median = stack_len(stack) / 2;
	while (stack)
	{
		stack->current_position = i;
		if (i <= median)
			stack->above_median = true;
		else
			stack->above_median = false;
		stack = stack->next;
		++i;
	}
}

/*
 * Optimized target finding - find target for all nodes in one pass
 */
static void	set_target_node(t_stack_node *a, t_stack_node *b)
{
	t_stack_node	*current_b;
	t_stack_node	*current_a;
	t_stack_node	*target_node;
	t_stack_node	*smallest_a;
	long			best_match_index;

	smallest_a = find_smallest(a); // Cache smallest node
	current_b = b;
	
	while (current_b)
	{
		best_match_index = LONG_MAX;
		target_node = smallest_a; // Default to smallest
		current_a = a;
		
		// Find the smallest number in 'a' that's bigger than current_b->nbr
		while (current_a)
		{
			if (current_a->nbr > current_b->nbr && current_a->nbr < best_match_index)
			{
				best_match_index = current_a->nbr;
				target_node = current_a;
			}
			current_a = current_a->next;
		}
		
		current_b->target_node = target_node;
		current_b = current_b->next;
	}
}

/*
 * Optimized price calculation
 */
void	set_price(t_stack_node *a, t_stack_node *b)
{
	int	len_a;
	int	len_b;
	t_stack_node *current_b;

	len_a = stack_len(a);
	len_b = stack_len(b);
	current_b = b;
	
	while (current_b)
	{
		// Cost to move current_b to top of stack b
		current_b->push_cost = current_b->current_position;
		if (!(current_b->above_median))
			current_b->push_cost = len_b - (current_b->current_position);
		
		// Add cost to move target to top of stack a
		if (current_b->target_node->above_median)
			current_b->push_cost += current_b->target_node->current_position;
		else
			current_b->push_cost += len_a - (current_b->target_node->current_position);
		
		current_b = current_b->next;
	}
}

/*
 * Optimized cheapest finding - single pass
 */
void	set_cheapest(t_stack_node *b)
{
	long			best_match_value;
	t_stack_node	*best_match_node;
	t_stack_node	*current;

	if (NULL == b)
		return ;
	
	// Reset all cheapest flags first
	current = b;
	while (current)
	{
		current->cheapest = false;
		current = current->next;
	}
	
	best_match_value = LONG_MAX;
	best_match_node = NULL;
	current = b;
	
	while (current)
	{
		if (current->push_cost < best_match_value)
		{
			best_match_value = current->push_cost;
			best_match_node = current;
		}
		current = current->next;
	}
	
	if (best_match_node)
		best_match_node->cheapest = true;
}

/*
 * Optimized initialization - calculate all values in optimal order
 */
void	init_nodes_a(t_stack_node *a, t_stack_node *b)
{
	if (!a || !b)
		return;
		
	set_current_position(a);
	set_current_position(b);
	set_target_node(a, b);
	set_price(a, b);
	set_cheapest(b);
}