/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: welepy <welepy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 13:03:41 by marcsilv          #+#    #+#             */
/*   Updated: 2025/02/19 14:56:21 by welepy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

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
	char	*single_q;
	char	*double_q;

	single_q = ft_strchr(input, '\'');
	double_q = ft_strchr(input, '\"');
	if (single_q && double_q)
	{
		if (single_q < double_q)
		{
			single_q++;
			if (!ft_strchr(single_q, '\''))
				return (false);
		}
		else
		{
			double_q++;
			if (!ft_strchr(double_q, '\"'))
				return (false);
		}
	}
	else if (!single_q && double_q)
	{
		double_q++;
		if (!ft_strchr(double_q, '\"'))
			return (false);
	}
	else if (!double_q && single_q)
	{
		single_q++;
		if (!ft_strchr(single_q, '\''))
			return (false);
	}
	return (true);
}

int	word_count_util(char *i, int count)
{
	char	quote;

	while (*i)
	{
		if (ft_isspace(*i))
			i++;
		else
		{
			if (*i == '\'' || *i == '\"')
			{
				quote = *i++;
				while (*i && *i != quote)
					i++;
				if (*i)
					i++;
			}
			else
				while (*i && !ft_isspace(*i) && \
				*i != '\'' && *i != '\"')
					i++;
			count++;
		}
	}
	return (count);
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

/*function count = 4*/
