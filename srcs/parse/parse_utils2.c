/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchingi <mchingi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 02:56:56 by mchingi           #+#    #+#             */
/*   Updated: 2025/02/15 12:20:22 by mchingi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../inc/minihell.h"

char	*fill_quote(char **input, char quote, int i)
{
	char	*quote_string;
	char	*temp;

	temp = strndup((*input) - i, i);
	quote_string = safe_malloc(sizeof(char) * (i + 3));
	quote_string[0] = quote;
	strncpy(quote_string + 1, temp, i);
	quote_string[i + 1] = quote;
	quote_string[i + 2] = '\0';
	free(temp);
	return (quote_string);
}

char	*extract_quote(char **input)
{
	int		i;
	char	quote;
	char	*quote_string;

	i = 0;
	quote = **input;
	(*input)++;
	while (**input)
	{
		if (**input == quote)
		{
			if (*(*input + 1) == quote)
				(*input)++;
			else
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

bool	extract_operator_util(char **input)
{
	int	i;

	i = 0;
	while ((*input)[i] == '*' && (*input)[i])
		i++;
	if (i > 1)
	{
		printf("invalid operator sequence in '*'\n");
		while (ft_strchr("*", **input) && **input)
			(*input)++;
		return (false);
	}
	else if (ft_strchr("|&*<>", **input) && **input)
	{
		printf("invalid operator sequence in '%c'\n", **input);
		while (ft_strchr("|&*<>", **input) && **input)
			(*input)++;
		return (false);
	}
	return (true);
}

char	*extract_operator(char **input)
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

int number_of_commands(t_token *tokens)
{
	t_token *tmp;
	int	number;

	number = 0;
	tmp = tokens;
	while (tmp)
	{
		if (tmp->type == COMMAND || is_builtin(tmp->type))
			number++;
		tmp = tmp->next;
	}
	return(number);
}