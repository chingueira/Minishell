/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: welepy <welepy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 10:55:56 by marcsilv          #+#    #+#             */
/*   Updated: 2025/02/20 18:55:18 by welepy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

static int	word_count(char *input)
{
	int	count;

	count = 0;
	count = word_count_util(input, count);
	return (count);
}

static char	*extract_command(char **input)
{
	char	*command;
	int		i;

	i = 0;
	while (**input && !ft_isspace(**input) && \
	!ft_strchr("|<>«»&*", **input) && **input != '\'' && **input != '\"')
	{
		i++;
		(*input)++;
	}
	command = ft_strndup((*input) - i, i);
	return (command);
}

static char	*extract_operator(char **input)
{
	char	*operator;
	char	temp_char;
	int		i;
	int		operator_size;

	temp_char = **input;
	operator_size = 0;
	i = 0;
	while ((**input == temp_char) && (operator_size < 2))
	{
		++operator_size;
		(*input)++;
	}
	if (!extract_operator_util(input))
		return (NULL);
	operator = safe_malloc(sizeof(char) * (operator_size + 1));
	while (i < operator_size)
		operator[i++] = temp_char;
	operator[i] = '\0';
	return (operator);
}

static char	*extract_quote(char **input)
{
	int		i;
	char	quote;
	char	*quote_string;

	i = 0;
	quote = **input;
	(*input)++;
	while (**input)
	{
		/*ft_putchar(*(*input));
		ft_putchar('\n');
		if (*(*input) != '\0' || !ft_isspace(*(*input)))
			(*input)++;
		else
			break ;*/
		if (**input == quote)
		{
			// if (*(*input + 1) == quote)
			// 	(*input)++;
			// else
				break ;
		}
		(*input)++;
		i++;
	}
	quote_string = fill_quote(input, quote, i);
	if (**input)
		(*input)++;
	return (quote_string);
}

char	**split_input(char *input, t_shell *shell)
{
	char	**matrix;
	int		y;

	y = 0;
	matrix = safe_malloc(sizeof(char *) * (word_count(input) + 1));
	if (!validate_quote_number(input))
	{
		ft_fprintf(2, "Error: can't parse unclosed quotes\n");
		shell->flag = false;
	}
	while (*input)
	{
		while ((ft_isspace(*input)) && *input)
			input++;
		if (*input == '\'' || *input == '\"')
			matrix[y++] = extract_quote(&input);
		else if (ft_strchr("|<>*&", *input))
			matrix[y++] = extract_operator(&input);
		else if (ft_isalnum(*input) || *input == '_' || *input == '/' \
		|| *input == '.' || *input == '-' || *input == '~' || 
		*input == ';' || *input == '.' || *input == ',' || *input == '\\')
			matrix[y++] = extract_command(&input);
		else if (*input == '$')
			matrix[y++] = extract_variable(&input);
	}
	matrix[y] = NULL;
	return (matrix);
}

