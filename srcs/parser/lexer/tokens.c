/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: welepy <welepy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 13:34:10 by marcsilv          #+#    #+#             */
/*   Updated: 2025/02/16 17:23:38 by welepy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

t_token	*new_token(char *value, t_type type, t_group_type group_type)
{
	t_token	*new_token;

	new_token = safe_malloc(sizeof(t_token));
	new_token->value = ft_strdup(value);
	new_token->type = type;
	new_token->group_type = group_type;
	new_token->next = NULL;
	return (new_token);
}
char	*token_kind_string(t_type type)
{
	if (type == GREATER)
		return ("Greater");
	else if (type == LESSER)
		return ("Lesser");
	else if (type == DOUBLE_GREATER)
		return ("D_Greater");
	else if (type == DOUBLE_LESSER)
		return ("Here-doc");
	else if (type == PIPE)
		return ("Pipe");
	else if (type == DOUBLE_AND)
		return ("D_AND");
	else if (type == DOUBLE_PIPE)
		return ("D_PIPE");
	else if (type == STAR)
		return ("STAR");
	else if (type == COMMAND)
		return ("COMMAND");
	else if (type == ARGUMENT)
		return ("ARGUMENT");
	else if (type == VARIABLE)
		return ("VARIABLE");
	else if (type == SINGLE_QUOTE)
		return ("SINGLE_QUOTE");
	else if (type == DOUBLE_QUOTE)
		return ("DOUBLE QUOTE");
	else if (type == ECHO)
		return ("echo");
	else if (type == CD)
		return ("cd");
	else if (type == PWD)
		return ("pwd");
	else if (type == EXPORT)
		return ("export");
	else if (type == UNSET)
		return ("unset");
	else if (type == ENV)
		return ("env");
	else if (type == EXIT)
		return ("exit");
	else if (type == OPTION)
		return ("OPTION");
	else if (type == PATH)
		return ("PATH");
	else if (type == IDENTIFIER)
		return ("IDENTIFIER");
	else
		return ("UNKNOWN");
}

char	*GroupKindString(t_group_type type)
{
	if (type == G_COMMAND)
		return ("COMMAND");
	else if (type == G_REDIRECTION)
		return ("REDIRECTION");
	else if (type == G_OPERATOR)
		return ("OPERATOR");
	else if (type == G_IDENTIFIER)
		return ("IDENTIFIER");
	else if (type == G_SINGLE_QUOTE)
		return ("SINGLE_QUOTE");
	else if (type == G_DOUBLE_QUOTE)
		return ("DOUBLE_QUOTE");
	return ("UNKNOWN");
}

t_group_type	find_group_type(char *value)
{
	if (value[0] == '\'' || ft_strchr(value, '\''))
		return (G_SINGLE_QUOTE);
	else if (value[0] == '\"' || ft_strchr(value, '\"'))
		return (G_DOUBLE_QUOTE);
	else if (ft_strspn(value, ALL) == ft_strlen(value))
		return (G_COMMAND);
	else if (ft_strspn(value, "<><<>>«»") == ft_strlen(value))
		return (G_REDIRECTION);
	else if (ft_strspn(value, "|&&||") == ft_strlen(value))
		return (G_OPERATOR);
	return (G_IDENTIFIER);
}

t_type	find_type(char *value)
{
	if (!ft_strcmp(value, ">"))
		return (GREATER);
	else if (!ft_strcmp(value, "<"))
		return (LESSER);
	else if (!ft_strcmp(value, ">>") || !ft_strcmp(value, "»"))
		return (DOUBLE_GREATER);
	else if (!ft_strcmp(value, "<<") || !ft_strcmp(value, "«"))
		return (DOUBLE_LESSER);
	else if (!ft_strcmp(value, "|"))
		return (PIPE);
	else if (!ft_strcmp(value, "&&"))
		return (DOUBLE_AND);
	else if (!ft_strcmp(value, "||"))
		return (DOUBLE_PIPE);
	else if (!ft_strcmp(value, "*"))
		return (STAR);
	else if (!ft_strcmp(value, "echo"))
		return (ECHO);
	else if (!ft_strcmp(value, "cd"))
		return (CD);
	else if (!ft_strcmp(value, "pwd"))
		return (PWD);
	else if (!ft_strcmp(value, "export"))
		return (EXPORT);
	else if (!ft_strcmp(value, "unset"))
		return (UNSET);
	else if (!ft_strcmp(value, "env"))
		return (ENV);
	else if (!ft_strcmp(value, "exit"))
		return (EXIT);
	else if (value[0] == '\'')
		return (SINGLE_QUOTE);
	else if (value[0] == '\"')
		return (DOUBLE_QUOTE);
	else if (strchr(value, '/'))
		return (PATH);
	else if (value[0] == '-')
		return (OPTION);
	else
		return (IDENTIFIER);
}
//validate if the token is a command or an argument
//validate if path is correct

t_token	*tokenize_matrix(char **matrix)
{
	t_token	*token;
	t_token	*new;
	int		i;

	i = 0;
	token = NULL;
	while (matrix[i])
	{
		new = new_token(matrix[i], find_type(matrix[i]), find_group_type(matrix[i]));
		// if (!new)
		// {
		// 	free_tokens(token);
		// 	return (NULL);
		// }
		ft_lstadd_back((t_list **)&token, (t_list *)new);
		i++;
	}
	return (token);
}

bool is_command(char *value, char *path)
{
	char	**paths;
	int		i;
	char	*tmp;
	bool	found;

	found = false;
	i = 0;
	paths = ft_split(path, ':');
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i], "/");
		free(paths[i]);
		paths[i] = ft_strjoin(tmp, value);
		free(tmp);
		i++;
	}
	i = 0;
	while (paths[i])
	{
		if (access(paths[i], F_OK) == 0)
		{
			found = true;
			break;
		}
		i++;
	}
	free_matrix(paths);
	return (found);
}



void	identify_tokens(t_token *tokens, char *path)
{
	t_token	*head;

	head = tokens;
	while (head)
	{
		if (head->type == IDENTIFIER)
		{
			if (is_command(head->value, path))
				head->type = COMMAND;
			else
				head->type = ARGUMENT;
		}
		head = head->next;
	}
	if (tokens->type == ARGUMENT)
		printf("%s: command not found\n", tokens->value);
}


void	debug(t_token *token, int number_of_commands)
{
	t_token	*head;

	head = token;
	while (head)
	{
		// printf("value: %s, type: %s\n", token->value, token_kind_string(token->type));
		// token = token->next;
		// if (head->type == COMMAND || head->type == ARGUMENT || head->type == VARIABLE)
		printf("%s, type: %s\n", head->value, token_kind_string(head->type));
		// else
		// 	printf("type: %s\n", token_kind_string(head->type));
		head = head->next;
	}
	printf("Number of commands: %d\n", number_of_commands);
}


