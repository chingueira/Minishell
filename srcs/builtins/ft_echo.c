/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchingi <mchingi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 15:18:05 by mchingi           #+#    #+#             */
/*   Updated: 2025/01/29 09:08:04 by mchingi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h" 

static bool	is_valid_option(char *value)
{
	int	i;

	// Check if the string starts with '-'
	if (value[0] != '-')
		return (false);

	// Check if all characters after '-' are 'n'
	i = 1;
	while (value[i])
	{
		if (value[i] != 'n')
			return (false);
		i++;
	}

	return (true);
}

static bool	echo_aux(t_token *token, bool *option)
{
	t_token	*temp;
	t_token	*temp2;
	bool	is_first_argument = true;

	temp = token;

	// Handle valid options (e.g., "-n", "-nnnnn")
	while (temp && temp->type == OPTION && is_valid_option(temp->value))
	{
		*option = true;
		temp = temp->next;
	}

	// Convert invalid options and subsequent ones to arguments
	temp2 = temp;
	while (temp2 && temp2->type == OPTION)
	{
		temp2->type = ARGUMENT;
		temp2 = temp2->next;
	}

	// Handle arguments and quotes
	while (temp && (temp->type == ARGUMENT || temp->type == SINGLE_QUOTE || temp->type == DOUBLE_QUOTE))
	{
		// Print space between arguments
		if (!is_first_argument)
			printf(" ");
		is_first_argument = false;

		// Print argument, ignoring quotes
		int i = 0;
		while (temp->value[i])
		{
			if (temp->value[i] != '\'' && temp->value[i] != '\"')
				printf("%c", temp->value[i]);
			i++;
		}

		temp = temp->next;
	}

	return (*option);
}

void	ft_echo(t_token *token)
{
	bool	option = false;

	if (!token)
	{
		write(1, "\n", 1);
		printf("\nfrom built-ins\n");
		return ;
	}

	option = echo_aux(token, &option);

	// Print newline unless -n option was used
	if (!option)
		printf("\n");
	printf("\nfrom built-ins\n");
}

