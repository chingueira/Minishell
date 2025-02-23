/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: welepy <welepy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 16:02:16 by mchingi           #+#    #+#             */
/*   Updated: 2025/02/23 14:36:06 by welepy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	ft_cd(t_token *current)
{
	static char 	*prev_path = NULL;
	char	*cur_path;
	char	*path;
	char	*new_path;

	cur_path = getcwd(NULL, 0);
	if (current->next)
	{
		printf("cd: too many arguments\n");
		printf("\nfrom built-ins\n");
		ft_free(&cur_path);
		g_exit_status = 1;
		return ;
	}
	if (ft_strcmp(current->value, "-") == 0)
	{
		if (!prev_path)
		{
			ft_fprintf(2, "cd: OLDPWD not set\n");
			g_exit_status = 1;
		}
		else
		{
			if (chdir(prev_path) == 0)
				printf("%s\n", prev_path);
			else
				perror("cd");
			g_exit_status = 0;
		}
		ft_free(&prev_path);
		prev_path = cur_path;
		printf("\nfrom built-ins\n");
		return ;
	}
	if (chdir(current->value) == 0)
	{
		printf("\nfrom built-ins\n");
		g_exit_status = 0;
		ft_free(&cur_path);
		return ;
	}
	path = ft_strjoin(cur_path, "/");
	new_path = ft_strjoin(path, current->value);
	ft_free(&path);
	if (*current->value == '~')
	{
		ft_free(&new_path);
		new_path = ft_strjoin(getenv("HOME"), current->value + 1);
	}
	if (chdir(new_path) == -1)
	{
		printf("cd: %s: No such file or directory\n", current->value);
		g_exit_status = 1;
	}
	g_exit_status = 0;
	ft_free(&new_path);
	ft_free(&cur_path);
}

