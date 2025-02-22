/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchingi <mchingi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 11:47:02 by mchingi           #+#    #+#             */
/*   Updated: 2025/02/19 01:13:47 by mchingi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../inc/minihell.h"

static char	*extract_command(char **input)
{
	char	*command;
	int		i;

	i = 0;
	while (**input && !ft_isspace(**input) && \
	!ft_strchr("|<>«»&*", **input))
	{
		i++;
		(*input)++;
	}
	command = ft_strndup((*input) - i, i);
	return (command);
}

char	*extract_variable(char **input)
{
	char	*variable;
	int		i;
	char	*temp;

	i = 0;
	(*input)++;
	while (**input && (ft_isalnum(**input) || **input == '_'))
	{
		i++;
		(*input)++;
	}
	temp = ft_strndup((*input) - i, i);
	variable = safe_malloc(sizeof(char) * (i + 2));
	variable[0] = '$';
	ft_strncpy(variable + 1, temp, i);
	variable[i + 1] = '\0';
	free(temp);
	return (variable);
}

char	**split_input(char *input, t_shell *shell)
{
	int		i;
	char	**array;

	i = 0;
	array = malloc(sizeof(char *) * (word_count(input) + 1));
	if (!validate_quote_number(input))
	{
		write(2, "Error: Can't parse unclosed quotes\n", 36);
		shell->flag = false;
		return (NULL);
	}
	while (*input)
	{
		while (isspace(*input) && *input)
			input++;
		if (*input == '\'' || *input == '\"')
			array[i++] = extract_quote(&input);
		else if (strchr("|<>*&", *input))
			array[i++] = extract_operator(&input);
		else if (ft_isalnum(*input) || *input == '_' || *input == '/' \
		|| *input == '.' || *input == '-' || *input == '~' || 
		*input == ';' || *input == '.' || *input == ',' || *input == '\\')
			array[i++] = extract_command(&input);
		else if (*input == '$')
			array[i++] = extract_variable(&input);
	}
	array[i] = NULL;
	return (array);
}

void	parse(t_shell *shell)
{
	shell->array = split_input(shell->input, shell);
	if (!shell->array)
	{
		write(2, "Error: Failed to parse input\n", 30);
		shell->flag = false;
	}
	expand(&shell->array, shell->env);	
	shell->token = tokenize_array(shell->array);
	if (!shell->token)
		error_message("token");
	identify_tokens(shell->token, shell->path);				//identifica os tokens
	token_sequence(shell->token);
	shell->num_of_cmds = number_of_commands(shell->token);
	shell->flag = true;
}
