#include "push_swap.h"

// Optimized chunk-based sorting for large datasets
static void	sort_large_stack(t_stack_node **a, t_stack_node **b)
{
	int	len = stack_len(*a);
	int	chunk_size;
	int	chunks;
	int	i;
	
	// Determine optimal chunk size based on stack length
	if (len <= 100)
		chunk_size = len / 3;
	else if (len <= 500)
		chunk_size = len / 5;
	else
		chunk_size = len / 8;
	
	chunks = (len + chunk_size - 1) / chunk_size; // Ceiling division
	
	// Push elements to b in chunks, largest first
	for (i = chunks - 1; i >= 0; i--)
	{
		int min_val = (len * i) / chunks + 1;
		int max_val = (len * (i + 1)) / chunks;
		
		// Push all elements in current chunk range
		while (stack_len(*a) > 3)
		{
			// Find a node in current chunk range
			t_stack_node *current = *a;
			bool found = false;
			
			while (current)
			{
				if (current->nbr >= min_val && current->nbr <= max_val)
				{
					found = true;
					break;
				}
				current = current->next;
			}
			
			if (!found)
				break;
			
			// Move the found element to top and push
			while ((*a)->nbr < min_val || (*a)->nbr > max_val)
			{
				// Simple rotation to find next element in range
				if ((*a)->next && 
					(((*a)->next->nbr >= min_val && (*a)->next->nbr <= max_val) ||
					 ((*a)->prev && (*a)->prev->nbr >= min_val && (*a)->prev->nbr <= max_val)))
					ra(a, true);
				else
					rra(a, true);
			}
			pb(b, a, true);
		}
	}
}

static void	rotate_both(t_stack_node **a,
						t_stack_node **b,
						t_stack_node *cheapest_node) //Define a function that rotates both the top `a` and `b` nodes to the bottom of their stacks, if it's the cheapest move
{
	while (*b != cheapest_node->target_node
		&& *a != cheapest_node) //As long as the current `b` node is not `a` cheapest node's target node, and the current top `a` node is not the top node
		rr(a, b, true); //Rotate both `a` and `b` nodes
	set_current_position(*a); //Refresh current node positions
	set_current_position(*b);
}

static void	rev_rotate_both(t_stack_node **a,
								t_stack_node **b,
								t_stack_node *cheapest_node) //Define a function that rotates both the bottom `a` and `b` nodes to the top of their stacks, if it's the cheapest move
{
	while (*b != cheapest_node->target_node
		&& *a != cheapest_node) //As long as the current `b` node is not `a` cheapest node's target node && and the current `a` node is not the cheapest
		rrr(a, b, true); //Reverse rotate both `a` and `b` nodes
	set_current_position(*a); //Refresh current node positions
	set_current_position(*b);
}

static void	move_a_to_b(t_stack_node **a, t_stack_node **b) //Define a function that prepares the cheapest nodes on top of the stacks for pushing `a` nodes to stack `b`, until there are three nodes left in `a`
{
	t_stack_node	*cheapest_node; //To store the pointer to the cheapest `a` node

	if (!a || !b || !*a) // OPTIMIZATION: Added null checks
		return;
	cheapest_node = return_cheapest(*a); //Get the cheapest node from stack `a`
	if (!cheapest_node) // OPTIMIZATION: Check if cheapest node exists
		return;
	// OPTIMIZATION: Handle all rotation cases more efficiently
	if (cheapest_node->above_median == cheapest_node->target_node->above_median)
	{
		if (cheapest_node->above_median) //Both above median
			rotate_both(a, b, cheapest_node);
		else //Both below median
			rev_rotate_both(a, b, cheapest_node);
	}
	else // OPTIMIZATION: Handle mixed median cases separately
	{
		if (cheapest_node->above_median)
			ra(a, true);
		else
			rra(a, true);
		if (cheapest_node->target_node->above_median)
			rb(b, true);
		else
			rrb(b, true);
	}
	prep_for_push(a, cheapest_node, 'a'); //Ensure the cheapest node is at the top, ready for pushing
	prep_for_push(b, cheapest_node->target_node, 'b'); //Ensure the target node is at the top, ready for pushing
	pb(b, a, true); //Push from `a` to `b`
}

static void	move_b_to_a(t_stack_node **a, t_stack_node **b) //Define a function that prepares `b`'s target `a` nodes for pushing all `b` nodes back to stack `a` 
{
	if (!b || !*b) // OPTIMIZATION: Added null check
		return;
	prep_for_push(a, (*b)->target_node, 'a'); //Ensure `b`'s target `a` node is on top of the stack
	pa(a, b, true); //Push from `b` to `a`
}

static void	min_on_top(t_stack_node **a) //Define a function that moves the smallest number to the top
{
	t_stack_node	*smallest; // OPTIMIZATION: Cache smallest node

	if (!a || !*a) // OPTIMIZATION: Added null check
		return;
	smallest = find_smallest(*a); //Find once instead of repeatedly
	if (!smallest)
		return;
	while ((*a)->nbr != smallest->nbr) //As long as the smallest number is not at the top
	{
		if (smallest->above_median) //Rotate or reverse rotate according to the position of the node on the median
			ra(a, true);
		else
			rra(a, true);
	}
}

// Optimized sorting function with different strategies based on size
void	sort_stacks(t_stack_node **a, t_stack_node **b)
{
	int	len_a;

	if (!a || !b)
		return;
	len_a = stack_len(*a);
	
	// Use different algorithms based on stack size
	if (len_a <= 50)
	{
		// Original Turk algorithm for small-medium stacks
		while (len_a > 3 && !stack_sorted(*a))
		{
			if (len_a == 4 || len_a == 5)
			{
				pb(b, a, true);
			}
			else
			{
				init_nodes_a(*a, *b);
				move_a_to_b(a, b);
			}
			len_a = stack_len(*a);
		}
	}
	else
	{
		// Optimized chunk-based algorithm for large stacks
		sort_large_stack(a, b);
	}
	
	sort_three(a);
	
	// Push everything back from b to a
	while (*b)
	{
		init_nodes_b(*a, *b);
		move_b_to_a(a, b);
	}
	
	set_current_position(*a);
	min_on_top(a);
}