/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchingi <mchingi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 21:08:38 by mchingi           #+#    #+#             */
/*   Updated: 2025/02/21 13:44:02 by mchingi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../inc/minihell.h"

void	free_array(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}

char	*find_path(char *cmd, char **envp)
{
	int		i;
	char	*path;
	char	*part_path;
	char	**paths;

	i = 0;
	while (envp[i] && ft_strnstr(envp[i], "PATH", 4) == 0)
		i++;
	paths = ft_split(envp[i] + 5, ':');
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		part_path = ft_strjoin(paths[i], "/");
		path = ft_strjoin(part_path, cmd);
		free(part_path);
		if (access(path, F_OK) == 0)
		{
			free_array(paths);
			return (path);
		}
		free(path);
		i++;
	}
	free_array(paths);
	return (NULL);
}


char	*clean_string(char *str)
{
	int 	tab[2];
	char	*cleaned_str;

	tab[0] = 0;
	tab[1] = 0;
	while (str[tab[0]])
	{
		if (str[tab[0]] != '\"' || str[tab[0]] != '\'')
			tab[1]++;
		tab[0]++;
	}
	cleaned_str = (char *)malloc(sizeof(char) * (tab[1] + 1));
	if (!cleaned_str)
		return (NULL);
	tab[0] = 0;
	tab[1] = 0;
	while (str[tab[0]])
	{
		if (str[tab[0]] == '\"' || str[tab[0]] == '\'')
			tab[0]++;
		cleaned_str[tab[1]] = str[tab[0]];
		tab[0]++;
		tab[1]++;
	}
	cleaned_str[tab[0]] = '\0';
	return (cleaned_str);
}

bool	is_redirection(t_type type)
{
	return (type == LESSER || type == GREATER ||
			type == HERE_DOC || type == APPEND);
}
bool	is_argument(t_type type)
{
	return (type == ARGUMENT || type == DOUBLE_QUOTE || type == SINGLE_QUOTE);
}

bool	is_option(t_type type)
{
	return (type == OPTION);
}

void	error_message(char *str)
{
	perror(str);
	exit(EXIT_FAILURE);
}
char	*eliminate_space_from_str(char *str)
{
	int	i = 0;
	int j = 0;
	char *new_str;

	while (ft_isspace(str[i]))
		i++;
	new_str = malloc(sizeof(str) + 1);
	while (str[i] && str[i] != ' ')
	{
		new_str[j] = str[i];
		i++;
		j++;
	}
	new_str[j] = '\0';
	return(new_str);
}
