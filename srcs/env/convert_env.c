/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   convert_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: welepy <welepy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 17:32:49 by marcsilv          #+#    #+#             */
/*   Updated: 2025/02/23 14:12:30 by welepy           ###   ########.fr       */
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

static void	free_env(t_env *head)
{
	t_env	*tmp;

	while (head)
	{
		tmp = head->next;
		ft_free(&head->name);
		ft_free(&head->value);
		if (head)
		{
			free(head);
			head = NULL;
		}
		head = tmp;
	}
}

void free_tokens(t_token *head)
{
	t_token	*tmp;

	while (head)
	{
		tmp = head->next;
		ft_free(&head->value);
		if (head)
		{
			free(head);
			head = NULL;
		}
		head = tmp;
	}
}

void	free_cmds(t_command *head)
{
	t_command	*tmp;

	while (head)
	{
		tmp = head->next;
		ft_free(&head->command);
		free_matrix(head->args);
		ft_free(&head->path);
		if (head)
		{
			free(head);
			head = NULL;
		}
		head = tmp;
	}
}

void	clean_or_exit(t_shell *shell, bool exit_flag)
{
	free_env(shell->env);
	ft_free(&shell->input);
	free_tokens(shell->token);
	free_matrix(shell->matrix);
	free_cmds(shell->command_groups);
	ft_free(&shell->path);
	rl_clear_history();
	ft_free(&shell->current_dir);
	// ft_free(&shell->pipe->ev);
	// ft_free(&shell->pipe);
	// free_matrix(shell->anv);
	if (exit_flag)
		exit(g_exit_status);
}

//find allocated variables that where copied, and free them

char	*get_path(t_env *env)
{
	t_env *tmp;

	tmp = env;
	while (tmp)
	{
		if (ft_strcmp(tmp->name, "PATH") == 0)
			return (ft_strdup(tmp->value));
		tmp = tmp->next;
	}
	return (NULL);
}
