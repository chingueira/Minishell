/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_tokens.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 14:29:13 by marcsilv          #+#    #+#             */
/*   Updated: 2025/02/15 11:34:25 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

void	identify(t_token **token, t_env *env)
{
	t_token	*head;
	t_env *temp;
	char	**matrix;

	head = *token;
	while (head)
	{
		if (head->type == IDENTIFIER)
		{
			temp = env;
			while (temp)
			{
				if (!ft_strcmp(temp->name, "PATH"))
				{
					if (ft_strstr(temp->value, head->value))
						head->type = COMMAND;
					else
						print_error("not found", NULL);
				}
				temp = temp->next;
			}
		}
		head = head->next;
	}
	free_matrix(matrix);
}

void	get_arguments(t_token **token)
{
	t_token	*head;
	char	**matrix;

	head = *token;
	while (head)
	{
		if (head->type == COMMAND)
		{
			if (head->next && head->next->group_type == G_REDIRECTION)
				print_error("syntax error", NULL);
			else if (head->next && !(head->next->group_type == G_OPERATOR))
				head->next->type = ARGUMENT;
			else
				print_error("define behavior for this case\n", NULL);
		}
		head = head->next;
	}
}

void	validate_token_sequence(t_token *tokens)
{
	t_token	*head;

	head = tokens;
	while (head)
	{
		if (head->type == G_COMMAND)
			if (head->next->group_type == G_COMMAND || head->next->group_type == G_REDIRECTION)
				print_error("invalid syntax", NULL);
		if (head->type == G_OPERATOR)
			if (head->prev->group_type != G_COMMAND || head->next->group_type != G_COMMAND)
				print_error("invalid syntax", NULL);
		if (head->type == G_REDIRECTION)
			if (head->prev->group_type != G_COMMAND || head->next->group_type != G_COMMAND)
				print_error("invalid syntax", NULL);
		if (head->type == G_OPERATOR)
			if (head->prev == NULL || head->next == NULL)
				print_error("invalid syntax", NULL);
		head = head->next;
	}
}

//echo ls cat || &&

static bool	is_builtin_or_command(t_type type)
{
	return (type == COMMAND || type == ECHO || type == CD || type == PWD ||
		type == EXPORT || type == UNSET || type == ENV);
}

void	token_sequence(t_token *tokens)
{
	t_token	*head;

	head = tokens;
	while (head)
	{
		if (is_builtin_or_command(head->type) && head->next && is_builtin_or_command(head->next->type))
		{
			while (head->next && (is_builtin_or_command(head->next->type)))
			{
				head->next->type = ARGUMENT;
				head = head->next;
			}
		}
		head = head->next;
	}
	head = tokens;
	while (head)
	{
		if (head->type == LESSER || head->type == GREATER || head->type == APPEND)
			if (head->next)
			{
				head = head->next;
				head->type = ARGUMENT;
				if (head->next && !(head->next->type == LESSER || head->next->type == GREATER || head->next->type == APPEND || head->next->type == PIPE))
					head->next->type = COMMAND;
			}
		head = head->next;
	}
}

void	validate_tokens(t_shell *shell)
{
	t_token	*head;

	head = shell->token;
	// identify(&shell->token, shell->env);
	// get_arguments(&shell->token);
	// validate_token_sequence(shell->token);

}
