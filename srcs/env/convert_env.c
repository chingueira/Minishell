/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   convert_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcsilv <marcsilv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 17:32:49 by marcsilv          #+#    #+#             */
/*   Updated: 2025/01/16 13:03:09 by marcsilv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h" 

t_env	*convert_env(char **env)
{
	t_env	*list;
	t_env	*new;
	int		i;

	list = NULL;
	i = 0;
	while (env[i])
	{
		new = malloc(sizeof(t_env));
		if (!new)
		{
			ft_lstclear((t_list **)&list, free);
			return (NULL);
		}
		new->name = ft_strndup(env[i], ft_strcspn(env[i], "="));
		new->value = ft_strdup(env[i] + ft_strcspn(env[i], "=") + 1);
		new->next = NULL;
		ft_lstadd_back((t_list **)&list, (t_list *)new);
		i++;
	}
	return (list);
}

char	*get_path(t_env *env)
{
	t_env *tmp;

	tmp = env;
	while (tmp)
	{
		if (ft_strcmp(tmp->name, "PATH") == 0)
			return (tmp->value);
		tmp = tmp->next;
	}
	return (NULL);
}
