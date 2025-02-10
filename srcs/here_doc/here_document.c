/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_document.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchingi <mchingi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 10:56:07 by mchingi           #+#    #+#             */
/*   Updated: 2025/02/10 10:52:44 by mchingi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/*
	Armazenar os inputs num file;
	Executar o comando que antecede << tendo como STDIN o file;
	Eliminar o file;
*/

#include <strings.h>

// ---------- Getting the Delimeter -----------------------------

static int	ft_wordlen(char const *s, int start)
{
	int	i;
	int	count;

	i = start;
	count = 0;
	while (s[i])
	{
		count++;
		i++;
	}
	return (count);
}

static char	*get_string(char *input, int start)
{
	int		i;
	int		j;
	int		len;
	char	*word;

	i = start;
	j = 0;
	len = ft_wordlen(input, i);
	word = (char *)malloc(sizeof(char) * (len + 1));
	if (!word)
		return (NULL);
	while (isspace(input[i]))
		i++;
	while (input[i] != ' ' && input[i] != '\0')
		word[j++] = input[i++];
	word[j] = '\0';
	return (word);
}

static char	*get_delimeter(char *input)
{
	int				i = 0;
	char	*delimeter;

	while (input[i])
	{
		if ((input[i] == '<' && input[i + 1] == '<'))
		{
			i++;
			i++;
			break ;
		}
		i++;
	}
	delimeter = get_string(input, i);
	return (delimeter);
}

//----------------------------------------------------------

void	here_doc(char *str)
{
	int		fd;
	// char	**cmd;
	char	*input;
	char	*delimeter;

	delimeter = get_delimeter(str);
	fd = open("DOC_TMP", O_WRONLY | O_CREAT | O_APPEND, 0777);
	while (1)
	{
		input = readline("> ");
		if (!input || strcmp(input, delimeter) == 0)
		{
			free(input);
			break ;
		}
		if (*input)
			add_history(input);
		write(fd, input, strlen(input));
		write(fd, "\n", 1);
		free(input);
	}
	close(fd);
}

int		check_doc(char *input)
{
	int	i = 0;
	while (input[i])
	{
		if (input[i] == '<' && input[i + 1] == '<')
			return (1);
		i++;
	}
	return (0);
}

/*int	main(int ac, char **av, char **ev)
{
	char	*input;

	(void) ac;
	(void) av;
	(void) ev;
	while (1)
	{
		input = readline("minihell> ");
		if (input == NULL)
			break ;
		if (*input)
			add_history(input);
		if (check_doc(input))
		{
			here_doc(input);
			
		}
		free(input);
	}
	return (0);
}*/