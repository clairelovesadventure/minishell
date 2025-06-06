/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marrey <marrey@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/01 00:00:00 by shutan            #+#    #+#             */
/*   Updated: 2025/05/12 22:13:03 by marrey           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* 创建新的环境变量 */
static t_env	*new_env(char *key, char *value)
{
	t_env	*env;

	env = (t_env *)malloc(sizeof(t_env));
	if (!env)
		return (NULL);
	env->key = ft_strdup(key);
	if (value)
		env->value = ft_strdup(value);
	else
		env->value = NULL;
	env->next = NULL;
	return (env);
}

/* 添加环境变量到链表 */
static void	add_env(t_env **env_list, t_env *new_env)
{
	t_env	*current;

	if (!*env_list)
	{
		*env_list = new_env;
		return ;
	}
	current = *env_list;
	while (current->next)
		current = current->next;
	current->next = new_env;
}

/* 解析环境变量字符串 */
static void	parse_env(t_env **env_list, char *env_str)
{
	int		i;
	char	*key;
	char	*value;

	i = 0;
	while (env_str[i] && env_str[i] != '=')
		i++;
	if (!env_str[i])
	{
		add_env(env_list, new_env(env_str, NULL));
		return ;
	}
	key = ft_substr(env_str, 0, i);
	value = ft_substr(env_str, i + 1, ft_strlen(env_str) - i - 1);
	add_env(env_list, new_env(key, value));
	free(key);
	free(value);
}

/* 初始化环境变量链表 */
t_env	*init_env(char **envp)
{
	t_env	*env_list;
	int		i;

	env_list = NULL;
	i = 0;
	while (envp[i])
	{
		parse_env(&env_list, envp[i]);
		i++;
	}
	return (env_list);
}

/* 获取环境变量值 */
char	*get_env_value(t_env *env_list, const char *key)
{
	t_env	*current;

	current = env_list;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

/* 设置环境变量值 */
void	set_env_value(t_env **env_list, char *key, char *value)
{
	t_env	*current;

	current = *env_list;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			if (current->value)
				free(current->value);
			if (value)
				current->value = ft_strdup(value);
			else
				current->value = NULL;
			return ;
		}
		current = current->next;
	}
	add_env(env_list, new_env(key, value));
}

/* 添加或更新环境变量 */
void	add_or_update_env(t_env **env_list, const char *arg)
{
	char	*key;
	char	*value;
	t_env	*current;
	t_env	*new_node;

	/* Split the argument into key and value */
	key = ft_strdup(arg);
	value = ft_strchr(key, '=');
	if (value)
	{
		*value = '\0';
		value++;
	}

	/* Search for the key in the environment list */
	current = *env_list;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			free(current->value);
			if (value)
				current->value = ft_strdup(value);
			else
				current->value = NULL;
			free(key);
			return ;
		}
		current = current->next;
	}

	/* If the key does not exist, add a new entry */
	new_node = malloc(sizeof(t_env));
	if (!new_node)
	{
		free(key);
		/* Consider proper error handling for malloc failure */
		return ;
	}
	new_node->key = key;
	if (value)
		new_node->value = ft_strdup(value);
	else
		new_node->value = NULL;
	new_node->next = *env_list;
	*env_list = new_node;
}

/* 释放环境变量链表 */
void	free_env(t_env *env_list)
{
	t_env	*tmp;

	while (env_list)
	{
		tmp = env_list;
		env_list = env_list->next;
		if (tmp->key)
			free(tmp->key);
		if (tmp->value)
			free(tmp->value);
		free(tmp);
	}
}

/* 将环境变量链表转换为字符串数组 */
char	**env_to_array(t_env *env_list)
{
	int		count;
	char	**env_array;
	t_env	*current;
	int		i;
	char	*tmp;

	count = 0;
	current = env_list;
	while (current)
	{
		count++;
		current = current->next;
	}
	env_array = (char **)malloc(sizeof(char *) * (count + 1));
	if (!env_array)
		return (NULL);
	i = 0;
	current = env_list;
	while (current)
	{
		if (current->value)
		{
			tmp = ft_strjoin(current->key, "=");
			env_array[i] = ft_strjoin(tmp, current->value);
			free(tmp);
		}
		else
			env_array[i] = ft_strdup(current->key);
		current = current->next;
		i++;
	}
	env_array[i] = NULL;
	return (env_array);
}
