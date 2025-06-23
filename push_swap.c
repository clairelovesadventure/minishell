/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inabakka <inabakka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 13:04:17 by inabakka          #+#    #+#             */
/*   Updated: 2025/05/23 13:53:05 by inabakka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

int	main(int argc, char **argv) //Define a program that takes in a random input of numbers and sorts them in ascending order
{
	t_stack_node	*a; //To store a pointer to stack `a`
	t_stack_node	*b; //To store a pointer to stack `b`
	char			**split_argv; //To store split arguments if input is a single string
	bool			is_split; //Flag to track if we split the input string

	a = NULL; //Initialize stack pointers to NULL
	b = NULL;
	split_argv = NULL;
	is_split = false;
	
	// Handle argument validation
	if (argc == 1 || (argc == 2 && !argv[1][0])) //Check for no arguments or empty string argument
		return (1);
	else if (argc == 2) //Handle case where input is a single string of space-separated numbers
	{
		split_argv = ft_split(argv[1], ' '); //Split the string into individual number strings
		is_split = true; //Set flag indicating we used split
		if (!split_argv) //Check if split failed
			error_exit(&a, &b, NULL, false); //Clean up and exit on error
	}
	
	// Initialize stack A with input numbers
	if (!stack_init_a(&a, is_split ? split_argv : argv + 1, is_split)) //Pass either split array or argv
		error_exit(&a, &b, split_argv, is_split); //Exit if initialization fails
	
	// Perform sorting if stack isn't already sorted
	if (!stack_sorted(a)) //Only sort if necessary
	{
		if (stack_len(a) == 2) //Special case: just swap two elements
			sa(&a, true); //Swap with output
		else if (stack_len(a) == 3) //Optimal sort for three elements
			sort_three(&a);
		else //Turk algorithm for larger stacks
			sort_stacks(&a, &b); //Uses both stacks with optimal moves
	}
	
	// Clean up memory
	free_stack(&a); //Free stack A nodes
	if (is_split)
		free_split(split_argv); //Free split array if we created it
	return (0); //Successful execution
}