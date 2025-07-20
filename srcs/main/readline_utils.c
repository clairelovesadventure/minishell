/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marrey <marrey@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 03:08:00 by marrey            #+#    #+#             */
/*   Updated: 2025/07/20 03:11:44 by marrey           ###   ########.fr       */
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