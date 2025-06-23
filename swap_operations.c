/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   swap_operations.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inabakka <inabakka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 13:05:14 by inabakka          #+#    #+#             */
/*   Updated: 2025/05/23 17:11:31 by inabakka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void sa(t_stack_node **a, bool print)
{
    if (!a || !*a || !(*a)->next)
        return;
        
    t_stack_node *first = *a;
    t_stack_node *second = first->next;
    
    // Update pointers
    first->next = second->next;
    if (second->next)
        second->next->prev = first;
    
    second->next = first;
    second->prev = NULL;
    first->prev = second;
    
    // Update stack head
    *a = second;
    
    if (print)
        write(1, "sa\n", 3);
}

void sb(t_stack_node **b, bool print)
{
    if (!b || !*b || !(*b)->next)
        return;
        
    t_stack_node *first = *b;
    t_stack_node *second = first->next;
    
    // Update pointers
    first->next = second->next;
    if (second->next)
        second->next->prev = first;
    
    second->next = first;
    second->prev = NULL;
    first->prev = second;
    
    // Update stack head
    *b = second;
    
    if (print)
        write(1, "sb\n", 3);
}

void ss(t_stack_node **a, t_stack_node **b, bool print)
{
    sa(a, false);
    sb(b, false);
    if (print)
        write(1, "ss\n", 3);
}