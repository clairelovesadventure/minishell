/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marrey <marrey@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/01 00:00:00 by user              #+#    #+#             */
/*   Updated: 2025/05/13 16:46:13 by marrey           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* 创建新标记 */
static t_token	*new_token(char *value, t_token_type type)
{
	t_token	*token;

	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
	{
		free(value);
		return (NULL);
	}
	token->value = value;
	token->type = type;
	token->next = NULL;
	return (token);
}

/* 添加标记到链表 */
static void	add_token(t_token **tokens, t_token *new_token)
{
	t_token	*current;

	if (!*tokens)
	{
		*tokens = new_token;
		return ;
	}
	current = *tokens;
	while (current->next)
		current = current->next;
	current->next = new_token;
}

/* Commenting out unused function handle_quotes */
/*
static int	handle_quotes(char *input, int *i, char **word)
{
	char	quote;
	int		start;

	quote = input[*i];
	start = *i;
	(*i)++;
	while (input[*i])
	{
		if (input[*i] == quote)
		{
			(*i)++;
			*word = ft_strjoin(*word, ft_substr(input, start, *i - start));
			return (1);
		}
		(*i)++;
	}
	*word = ft_strjoin(*word, ft_substr(input, start, *i - start));
	return (0);
}
*/

/* 处理特殊字符 */
// Returns 1 on success (token added), 0 on error (malloc failure),
// -1 if input[*i] is not a special char handled by this function.
static int	handle_special_char(char *input, int *i, t_token **tokens)
{
	char *value_str;
	t_token *new_tok;

	if (input[*i] == '|')
	{
		value_str = ft_strdup("|");
		if (!value_str) return (0);
		new_tok = new_token(value_str, T_PIPE);
		if (!new_tok) return (0); // new_token frees value_str on failure
		add_token(tokens, new_tok);
		(*i)++;
		return (1);
	}
	else if (input[*i] == '<')
	{
		if (input[*i + 1] == '<')
		{
			value_str = ft_strdup("<<");
			if (!value_str) return (0);
			new_tok = new_token(value_str, T_HEREDOC);
			if (!new_tok) return (0);
			add_token(tokens, new_tok);
			(*i) += 2;
		}
		else
		{
			value_str = ft_strdup("<");
			if (!value_str) return (0);
			new_tok = new_token(value_str, T_REDIR_IN);
			if (!new_tok) return (0);
			add_token(tokens, new_tok);
			(*i)++;
		}
		return (1);
	}
	else if (input[*i] == '>')
	{
		if (input[*i + 1] == '>')
		{
			value_str = ft_strdup(">>");
			if (!value_str) return (0);
			new_tok = new_token(value_str, T_REDIR_APPEND);
			if (!new_tok) return (0);
			add_token(tokens, new_tok);
			(*i) += 2;
		}
		else
		{
			value_str = ft_strdup(">");
			if (!value_str) return (0);
			new_tok = new_token(value_str, T_REDIR_OUT);
			if (!new_tok) return (0);
			add_token(tokens, new_tok);
			(*i)++;
		}
		return (1);
	}
	return (-1); // Not a special char for this function
}

/* 处理单词或引用 */
// Returns 1 on success (token added or word was empty after processing),
// 0 on error (malloc failure).
static int	handle_word(char *input, int *i, t_token **tokens)
{
	char	*final_word_value;
	char	*segment;
	int		segment_start_index;
	char	quote_char;
	t_token *new_tok; // For checking new_token result

	final_word_value = ft_strdup("");
	if (!final_word_value)
		return (0); /* Malloc error */
	while (input[*i] && input[*i] != ' ' && input[*i] != '\t' && \
			input[*i] != '|' && input[*i] != '<' && input[*i] != '>')
	{
		char *temp_join_result; // Renamed from temp to avoid conflict if used elsewhere
		segment_start_index = *i;
		segment = NULL;
		if (input[*i] == '\'' || input[*i] == '\"')
		{
			quote_char = input[*i];
			(*i)++;
			while (input[*i] && input[*i] != quote_char)
				(*i)++;
			if (input[*i] == quote_char)
				(*i)++;
			segment = ft_substr(input, segment_start_index, *i - segment_start_index);
		}
		else /* Unquoted segment */
		{
			while (input[*i] && input[*i] != ' ' && input[*i] != '\t' && \
					input[*i] != '|' && input[*i] != '<' && input[*i] != '>' && \
					input[*i] != '\'' && input[*i] != '\"')
				(*i)++;
			segment = ft_substr(input, segment_start_index, *i - segment_start_index);
		}
		if (segment)
		{
			temp_join_result = final_word_value;
			final_word_value = ft_strjoin(temp_join_result, segment);
			free(temp_join_result);
			free(segment);
			if (!final_word_value)
				return (0); /* Malloc error from ft_strjoin */
		}
		else /* ft_substr likely failed */
		{
			free(final_word_value); // Free partially built word if substr failed
			return (0); // Indicate malloc error from ft_substr
		}
	}
	if (ft_strlen(final_word_value) > 0)
	{
		new_tok = new_token(final_word_value, T_WORD);
		if (!new_tok) // new_token already freed final_word_value if it failed
			return (0); // Malloc error for token struct
		add_token(tokens, new_tok);
	}
	else
		free(final_word_value); // Free if empty and no token created
	return (1); // Success
}

/* 词法分析主函数 */
t_token	*lexer(char *input)
{
	int		i;
	t_token	*tokens;
	int		status;

	i = 0;
	tokens = NULL;
	while (input[i])
	{
		if (input[i] == ' ' || input[i] == '\t')
		{
			i++;
			continue;
		}
		status = handle_special_char(input, &i, &tokens);
		if (status == 0) // Malloc error in handle_special_char
		{
			free_tokens(tokens);
			return (NULL);
		}
		if (status == 1) // Special char handled successfully
			continue;
		// If status == -1, it was not a special char, so try handle_word
		
		status = handle_word(input, &i, &tokens);
		if (status == 0) // Malloc error in handle_word
		{
			free_tokens(tokens);
			return (NULL);
		}
		// If handle_word returns 1, it succeeded (token may or may not have been added if word was empty)
		// The loop continues as i has been advanced by handle_word.
	}
	return (tokens);
}

/* 释放标记链表 */
void	free_tokens(t_token *tokens)
{
	t_token	*tmp;

	while (tokens)
	{
		tmp = tokens;
		tokens = tokens->next;
		if (tmp->value)
			free(tmp->value);
		free(tmp);
	}
}
