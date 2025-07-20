/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 19:30:08 by shutan            #+#    #+#             */
/*   Updated: 2025/06/08 09:46:32 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <readline/readline.h>

void	clear_readline_buffers(void)
{
	rl_clear_history();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	cleanup_readline_completely(void)
{
	rl_clear_history();
	rl_replace_line("", 0);
	rl_redisplay();
	rl_clear_history();
}
