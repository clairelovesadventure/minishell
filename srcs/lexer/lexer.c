/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 15:49:26 by shutan            #+#    #+#             */
/*   Updated: 2025/07/04 11:02:15 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	is_whitespace(char c)
{
	return (c == ' ' || c == '\t');
}

static int	is_operator(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

t_token	*lexer(char *input)
{
	int		i;
	t_token	*tokens;

	i = 0;
	tokens = NULL;
	while (input[i])
	{
		if (is_whitespace(input[i]))
			i++;
		else if (is_operator(input[i]))
			handle_special_char(input, &i, &tokens);
		else
			handle_word(input, &i, &tokens);
	}
	return (tokens);
}
