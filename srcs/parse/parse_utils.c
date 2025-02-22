/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchingi <mchingi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 00:43:22 by mchingi           #+#    #+#             */
/*   Updated: 2025/02/15 03:01:01 by mchingi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../inc/minihell.h"

int	word_count_util(char *input, int count)
{
	char	quote;

	while (*input)
	{
		if (isspace(*input))
			input++;
		else
		{
			if (*input == '\'' || *input == '\"')
			{
				quote = *input++;
				while (*input && *input != quote)
					input++;
				if (*input)
					input++;
			}
			else
				while (*input && !isspace(*input) && \
				*input != '\'' && *input != '\"')
					input++;
			count++;
		}
	}
	return (count);
}

int	word_count(char *input)
{
	int	count;

	count = 0;
	count = word_count_util(input, count);
	return (count);
}

int	count_char(char *input, char c)
{
	int	count;

	count = 0;
	while (*input)
	{
		if (*input == c)
			count++;
		input++;
	}
	return (count);
}

bool	validate_quote_number(char *input)
{
	int	count_single;
	int	count_double;

	count_single = count_char(input, '\'');
	count_double = count_char(input, '\"');
	if (count_single % 2 || count_double % 2)
		return (false);
	return (true);
}