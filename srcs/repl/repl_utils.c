/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   repl_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: welepy <welepy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 22:52:26 by welepy            #+#    #+#             */
/*   Updated: 2025/02/21 17:23:14 by marcsilv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

bool	pipe_check(t_shell *shell)
{
	t_token	*tmp;

	tmp = shell->token;
	while (tmp)
	{
		if (tmp->type == PIPE)
		{
			if (!tmp->next || tmp->next->type != COMMAND)
			{
				printf("Error: Invalid pipe\n");
				return (false);
			}
			return (true);
		}
		tmp = tmp->next;
	}
	return (false);
}

void	debug_commands(t_command *command_groups)
{
	t_command	*tmp;
	int			j;
	int			i;

	i = 0;
	j = 0;
	tmp = command_groups;
	while (tmp)
	{
		printf("Command %d: %s\n", i, tmp->command);
		printf("Args: ");
		while (tmp->args[j])
		{
			printf("%s ", tmp->args[j]);
			j++;
		}
		printf("\n");
		tmp = tmp->next;
		i++;
	}
}

static void	clean_execution(t_shell *shell)
{
	ft_free(&shell->input);
	free_matrix(shell->matrix);
	free_tokens(shell->token);
}

void	execute(t_shell *shell)
{
	t_token	*tmp;

	tmp = shell->token;
	if (is_builtin(tmp->type) || tmp->type == SINGLE_QUOTE
		|| tmp->type == DOUBLE_QUOTE)
		exec_builtins(shell);
	else if (tmp->type == COMMAND || tmp->type == PATH
		|| tmp->type == SINGLE_QUOTE || tmp->type == DOUBLE_QUOTE)
		exec_cmd(shell);
	clean_execution(shell);
}
