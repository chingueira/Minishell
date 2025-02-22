/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchingi <mchingi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 15:46:17 by mchingi           #+#    #+#             */
/*   Updated: 2025/02/16 17:27:57 by mchingi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../inc/minihell.h"

void	ft_cd_prev(char	*prev_path)
{
	if (!prev_path)
			printf("cd: OLDPWD not set\n");
	else
	{
		if (chdir(prev_path) == 0)
			printf("%s\n", prev_path);
		else
			perror("cd");
	}
}

void	ft_cd_aux(t_token *current, char *cur_path)
{
	char	*path;
	char	*new_path;

	path = ft_strjoin(cur_path, "/");
	new_path = ft_strjoin(path, current->value);
	if (*current->value == '~')
		new_path = ft_strjoin(getenv("HOME"), current->value + 1);
	if (chdir(new_path) == -1)
		printf("cd: %s: No such file or directory\n", current->value);
}

void	ft_cd(t_token *current)
{
	char		*cur_path;
	static char	*prev_path;
	t_token		*token;

	token = current->next;
	if (!token)
	{
		chdir(getenv("HOME"));
		return ;
	}
	cur_path = getcwd(NULL, 0);
	if (token->next)
	{
		ft_putstr_fd("cd: too many arguments\n", 2);
		return ;
	}
	if (strcmp(token->value, "-") == 0)
	{
		ft_cd_prev(prev_path);
		free(prev_path);
		prev_path = cur_path;
		return ;
	}
	ft_cd_aux(token, cur_path);
}
