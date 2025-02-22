/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchingi <mchingi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 12:53:03 by mchingi           #+#    #+#             */
/*   Updated: 2025/02/22 16:01:35 by mchingi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../inc/minihell.h"

int		arr_size(t_token *token)
{
	int	counter = 0;
	t_token *tmp = token;

	while (tmp)
	{
		if (is_command2(tmp->value))
		{
			counter++;
			while(tmp->next && is_argument(tmp->type))
			{
				counter++;
				tmp = tmp->next;
			}
			break ;
		}
		tmp=tmp->next;
	}
	return (counter);
}

char	**cmd_args(t_token *token)
{
	int		i;
	int		size;
	char	**args;
	t_token *head;

	i = 0;
	head = token;
	size = arr_size(token);
	args = malloc(sizeof(char *) * size + 1);
	if (is_command2(head->value))
	{
		while (head)
		{
			args[i++] = ft_strdup(head->value); 
			while(head->next && (is_argument(head->next->type) || is_option(head->next->type)))
			{
				args[i++] = ft_strdup(head->next->value);
				head = head->next;
			}
			break ;
		}
		head = head->next;
	}
	args[i] = NULL;
	return (args);
}

char	*file_name(t_token *token)
{
	char	*file_name;
	t_token	*tmp;

	tmp = token;
	while (tmp)
	{
		if (is_redirection(tmp->type) && tmp->next)
		{
			tmp = tmp->next;
			file_name = ft_strdup(tmp->value);
			break ;
		}
		tmp = tmp->next;
	}
	return (file_name);
}

char	**copy_array(char *str, char **array, int start)
{
	int	i = 1;
	int j = start;
	char	**copy;

	while (array[start] != NULL)
		start++;
	copy = malloc(sizeof(char *) * start + 1);
	copy[0] = strdup(str);
	while (array[j])
	{
		copy[i] = strdup(array[j]);
		i++;
		j++;
	}
	copy[i] = NULL;
	return (copy);
}

char	*remove_quotes(char *str)
{
	size_t len;
	if (str == NULL || strlen(str) < 2)
		return (str);
	len = ft_strlen(str);
	if ((str[0] == '"' || str[0] == '\'') && (str[len - 1] == '"' || str[len - 1] == '\''))
	{
		str++;
		str[len - 2] = '\0';
	}
	return (str);
}
