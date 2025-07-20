/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_operations.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marrey <marrey@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 09:34:22 by marrey            #+#    #+#             */
/*   Updated: 2025/07/10 14:21:30 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_OPERATIONS_H
# define ENV_OPERATIONS_H

# include "../../includes/minishell.h"
# include "env_utils.h"

void	add_or_update_env(t_env **env_list, const char *arg);
char	*get_env_value(t_env *env_list, const char *key);
void	set_env_value(t_env **env_list, char *key, char *value);

#endif
